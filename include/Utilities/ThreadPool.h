//!
//! ThreadPool.h
//! Manages a set of threads and delegates tasking
//! 
#pragma once

#include <vector>
#include "Thread.h"
#include "WorkerThread.h"
#include <queue>




namespace Util {

	template <class WorkerThreadType>
	class ThreadPool {
		using TaskType = typename WorkerThreadType::value_type;
		static_assert(isWorkerThread<WorkerThreadType>::value, "ThreadType must derive from WorkerThread<TaskType>");

	private:
		int nThreads;
		std::queue<TaskType> taskQueue;
		std::vector<std::unique_ptr<WorkerThreadType>> threads; 
		std::queue<int> idleThreadIdx;	// Shared
		std::string name;
		bool isActive;
		std::mutex taskQueueMutex, idlePoolMutex;
		std::condition_variable idleCond;

	public:
		//! Constructors
		//! 
		ThreadPool(std::string name, int nThreads)
			: name(name)
			, nThreads(nThreads)
			, isActive(false)
		{}

		//! Interface functions
		//! 
		void Init();
		void AddTasks(std::vector<TaskType>& tasks);
		void AddTask(TaskType& task);
		void WaitIdle();
		void Shutdown();
		
	};

	//! Init
	//! Initializes thread pool child threads to an active state
	//! 
	template <class WorkerThreadType>
	void ThreadPool<WorkerThreadType>::Init() {
		//! Instantiate threads
		for (int threadIdx = 0; threadIdx < nThreads; threadIdx++) {
			threads.push_back(std::make_unique<WorkerThreadType>(
				name + "_" + std::to_string(threadIdx), [threadIdx, this](WorkerThread<TaskType>* thread, bool success) {
					if (!success) {
						Util::Log::Error(name + "_" + std::to_string(threadIdx) + ": Failed to accomplish task");
					}

					std::lock_guard<std::mutex> lock(taskQueueMutex);

					if (!taskQueue.empty()) {
						//! Acquire new tasking
						TaskType task = std::move(taskQueue.front());
						taskQueue.pop();
						thread->SetTask(std::move(task));
					}
					else {
						//! Thread is now idle
						idleThreadIdx.push(threadIdx);

						//! Signal thread pool on empty task queue
						if (this->idleThreadIdx.size() == nThreads) {
							idleCond.notify_one();
						}
					}
				}
			));

			//! Add thread to idle queue
			idleThreadIdx.push(threadIdx);

			//! Start the thread
			threads[threadIdx]->Start(nullptr);	// TODO: Hide arguments from start()
			Util::Log::Debug(threads[threadIdx]->GetName() + ": Initialized thread");
		}

		isActive = true;
		Util::Log::Debug("ThreadPool: Successfully initialized thread pool with name " + name);
	}

	//! AddTasks
	//! Delegates a set of tasks to be processed by the thread pool
	//! 
	template <class WorkerThreadType>
	void ThreadPool<WorkerThreadType>::AddTasks(std::vector<TaskType>& tasks) {
		if (!isActive) {
			Util::Log::Warn("Attempted to add tasks on an inactive thread pool");
			return;
		}

		{
			std::lock_guard<std::mutex> lock(taskQueueMutex);

			//! Dispatch to all idle workers as applicable
			int nTasks = tasks.size();
			int idleThreadCount = this->idleThreadIdx.size();
			int nAssignments = std::min(nTasks, idleThreadCount);

			for (int taskI = 0; taskI < nAssignments; taskI++) {
				int threadIdx = idleThreadIdx.front();
				idleThreadIdx.pop();
				threads[threadIdx]->SetTask(std::move(tasks[taskI]));
				Util::Log::Debug(name + ": Added task (ID: " + std::to_string(tasks[taskI].GetUID()) + ") to worker thread (" + threads[threadIdx]->GetName() + ")");
			}

			//! Add remaining tasks to the pending queue
			for (int taskI = nAssignments; taskI < nTasks; taskI++) {
				taskQueue.push(std::move(tasks[taskI]));
				Util::Log::Debug(name + ": Added task (ID: " + std::to_string(tasks[taskI].GetUID()) + " to work queue");
			}
		}
	}

	//! AddTask
	//! Delegates a task to be processed by the thread pool
	//! 
	template <class WorkerThreadType>
	void ThreadPool<WorkerThreadType>::AddTask(TaskType& task) {
		if (!isActive) {
			Util::Log::Warn("Attempted to add a task on an inactive thread pool");
			return;
		}

		{
			std::lock_guard<std::mutex> lock(taskQueueMutex);

			//! Check for an idle worker to immediately provide the task
			if (!idleThreadIdx.empty()) {
				int threadIdx = idleThreadIdx.front();
				idleThreadIdx.pop();
				threads[threadIdx]->SetTask(std::move(task));
				Util::Log::Debug(name + ": Added task (ID: " + std::to_string(task.GetUID()) + ") to worker thread (" + threads[threadIdx]->GetName() + ")");
			}
			else {
				//! Add task to the pending queue since no worker is available
				taskQueue.push(std::move(task));
				Util::Log::Debug(name + ": Added task (ID: " + std::to_string(task.GetUID()) + " to work queue");
			}
		}
	}

	template <class WorkerThreadType>
	void ThreadPool<WorkerThreadType>::WaitIdle() {
		Util::Log::Debug(name + ": Waiting for tasks to complete");
		std::unique_lock<std::mutex> lock(idlePoolMutex);
		idleCond.wait(lock, [&]() { return idleThreadIdx.size() == nThreads; });
		Util::Log::Debug(name + ": Tasks complete");
	}

	//! Shutdown
	//! Shuts down all active threads in the pool
	//! 
	template <class WorkerThreadType>
	void ThreadPool<WorkerThreadType>::Shutdown() {
		for (auto& thread : threads) {
			thread->Stop();
		}
	}

}; // namespace Util