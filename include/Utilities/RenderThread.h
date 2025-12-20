//!
//! RenderThread.h
//! Defines a worker thread that processes rendering tasks
//! 
#pragma once

#include "WorkerThread.h"
#include "RenderTask.h"
namespace Renderer {
	class Renderer;
}



namespace Util {

	class RenderThread : public WorkerThread<RenderTask> {
	private:
		int resDownScale;

	public:
		RenderThread(std::string name, std::function<void(WorkerThread*, bool)> taskComplete_Callback);

	protected:
		bool Init() override;
		bool HandleTask() override;
	};

}; // namespace Util
