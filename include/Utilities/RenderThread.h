//!
//! RenderThread.h
//! Defines an worker thread that processes rendering tasks
//! 
#pragma once

#include "WorkerThread.h"
#include "RenderTask.h"



namespace Util {

	class RenderThread : public WorkerThread<RenderTask> {
	public:
		RenderThread(std::string name, std::function<void(WorkerThread*, bool)> taskComplete_Callback);

	protected:
		bool Init() override;
		bool HandleTask() override;
	};

}; // namespace Util
