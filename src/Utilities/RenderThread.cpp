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
		return true;
	}

	bool RenderThread::HandleTask() {
		/* ----------------------------------------------------------------
		 * Calculate total light for each ray
		 * ---------------------------------------------------------------- */
		const Util::RenderTask* taskRef = task.get();
		int startIdx = taskRef->startIdx;
		int endIdx = taskRef->endIdx;
		const std::vector<Renderer::RayMgr::Ray>* rays = taskRef->rays;
		Renderer::Renderer* renderer = taskRef->renderer;

		// TODO: This logic should be done in the renderer 
		for (int rayIdx = startIdx; rayIdx < endIdx; rayIdx++) {
			const Renderer::RayMgr::Ray& ray = (*rays)[rayIdx];
			Util::Vector3 color = renderer->CalcTotalLight(ray);
			
			//! Store final color
			Renderer::Frame* frame = renderer->GetRawFrame();
			uint32_t colorAdj = (int)color.x << 6 * 4 | (int)color.y << 4 * 4 | (int)color.z << 2 * 4 | 0xFF;
			frame->SetPixel(rayIdx % frame->GetWidth(), rayIdx / frame->GetWidth(), colorAdj);
		}

		return true;
	}

}; // namespace Util
