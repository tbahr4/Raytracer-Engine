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
		int maxThreads;
	public:
		std::queue<TaskType> taskQueue;
		std::vector<std::unique_ptr<WorkerThreadType>> threads; 
		std::queue<int> idleThreadIdx;	// Shared
		std::string name;
		bool isActive;
		std::mutex mutex;

	public:
		//! Constructors
		//! 
		ThreadPool(std::string name, int maxThreads)
			: name(name)
			, maxThreads(maxThreads)
			, isActive(false)
		{}

		//! Interface functions
		//! 
		void Init();
		void AddTask(TaskType& task);
		void Shutdown();
		
	};

	//! Init
	//! Initializes thread pool child threads to an active state
	//! 
	template <class WorkerThreadType>
	void ThreadPool<WorkerThreadType>::Init() {
		//! Instantiate threads
		for (int threadIdx = 0; threadIdx < maxThreads; threadIdx++) {
			threads.push_back(std::make_unique<WorkerThreadType>(
				name + "_" + std::to_string(threadIdx), [threadIdx, this](WorkerThread<TaskType>* thread, bool success) {
					if (!success) {
						Util::Log::Error(name + "_" + std::to_string(threadIdx) + ": Failed to accomplish task");
					}

					Util::Log::Info("Task complete callback received");

					std::lock_guard<std::mutex> lock(mutex);

					if (!taskQueue.empty()) {
						//! Acquire new tasking
						TaskType task = std::move(taskQueue.front());
						taskQueue.pop();
						thread->SetTask(std::move(task));
					}
					else {
						//! Thread is now idle
						idleThreadIdx.push(threadIdx);
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
			std::lock_guard<std::mutex> lock(mutex);

			//! Check for an idle worker to immediately provide the task
			if (!idleThreadIdx.empty()) {
				int threadIdx = idleThreadIdx.front();
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