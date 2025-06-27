//!
//! RenderThread.cpp
//! Defines an worker thread that processes rendering tasks
//! 
#include "RenderThread.h"




namespace Util {

	RenderThread::RenderThread(std::string name, std::function<void(WorkerThread*, bool)> taskComplete_Callback)
		: WorkerThread(name, taskComplete_Callback)
	{}

	bool RenderThread::Init() {
		// Custom setups for calculations (not starting the thread), if needed
		Util::Log::Info("In render thread init function");
		return true;
	}

	bool RenderThread::HandleTask() {
		// Custom implementation for handling task - which is RenderTask
		Util::Log::Info("In render thread handle task function");
		//this->task.get(). ...
		return true;
	}

}; // namespace Util
