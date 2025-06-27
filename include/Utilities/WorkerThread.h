//!
//! WorkerThread.h
//! Defines an worker thread that works multiple tasks
//! throughout its execution
//! 
#pragma once

#include "Thread.h"
#include <condition_variable>
#include <functional>
#include "Util.h"
#include "ThreadPool.h" // FIXME: separate



namespace Util {

	template <class TaskType>
	class WorkerThread : public Thread {
		static_assert(std::is_base_of<WorkTask, TaskType>::value, "TaskType must derive from WorkTask");

	public:
		using value_type = TaskType;
		using CallbackType = std::function<void(WorkerThread*, bool)>;

	private:
		std::atomic<int> lastTaskID;
		std::atomic<bool> awaitingTask;

		CallbackType taskComplete_Callback;	// Callback(this, success)
		std::mutex mutex;
		std::condition_variable cond;
		bool isStopped;

	public:
		WorkerThread(std::string name, std::function<void(WorkerThread*, bool)> taskComplete_Callback)
			: Thread(name)
			, isStopped(false)
			, lastTaskID(-1)	// TODO: Should probably make this safer
			, taskComplete_Callback(taskComplete_Callback)
			, awaitingTask(true)
		{}

		void SetTask(TaskType& task) {
			{
				std::lock_guard<std::mutex> lock(mutex);
				this->task = std::make_unique<TaskType>(std::move(task));
				awaitingTask.store(false);
			}
			cond.notify_one();
		}

		bool IsAwaitingTask() const {
			return awaitingTask.load();
		}

		void Stop() {
			{
				std::lock_guard<std::mutex> lock(mutex);
				isStopped = true;
			}
			cond.notify_one();
			this->Join();
		}

	protected:
		std::unique_ptr<TaskType> task;
		virtual bool HandleTask() = 0;
		
		int Run(void* vArgs) override {
			while (!isStopped) {
				//! Await new tasking
				{
					Util::Log::Debug(GetName() + ": Thread awaits new task");
					std::unique_lock<std::mutex> lock(mutex);
					cond.wait(lock, [&]() { return isStopped || (task && task->GetUID() != lastTaskID.load()); });

					if (isStopped && (!task || task->GetUID() == lastTaskID.load())) {
						return 0;
					}

					lastTaskID.store(task->GetUID());
					Util::Log::Debug(GetName() + ": Thread received new task (ID: " + std::to_string(task->GetUID()) + ")");
					Util::Log::Info("Thread running new task...");
				}

				//! Handle latest task
				bool success = HandleTask();
				Util::Log::Debug(GetName() + ": Thread completed task (ID: " + std::to_string(task->GetUID()) + ")");

				//! Perform callback
				if (taskComplete_Callback) {
					taskComplete_Callback(this, success);
				}

				if (!success) {
					Util::Log::Error(this->GetName() + ": WorkerThread failed to handle task with ID " + std::to_string(lastTaskID));
				}
			}

			return 0;
		}

	};

	//! WorkerThread trait
	//! Used to check whether Derived inherits from WorkerThread<T> for some T
	//! 
	template <typename Derived>
	struct isWorkerThread {
	private:
		template <typename T>
		static std::true_type test(const WorkerThread<T>*) { return {}; };
		static std::false_type test(...) { return {}; };

	public:
		static constexpr bool value = decltype(test(std::declval<Derived*>()))::value;
	};

}; // namespace Util
