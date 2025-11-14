//!
//! RenderThread.cpp
//! Defines an worker thread that processes rendering tasks
//! 
#include "RenderThread.h"




namespace Util {

	RenderThread::RenderThread(std::string name, int resDownScale, std::function<void(WorkerThread*, bool)> taskComplete_Callback)
		: WorkerThread(name, taskComplete_Callback)
		, resDownScale(resDownScale)
	{}

	bool RenderThread::Init() {
		return true;
	}

	bool RenderThread::HandleTask() {
		/* ----------------------------------------------------------------
		 * Calculate total light for each ray
		 * ---------------------------------------------------------------- */
		const Util::RenderTask* taskRef = task;
		int startIdx = taskRef->startIdx;
		int endIdx = taskRef->endIdx;
		const std::vector<Renderer::RayMgr::Ray>* rays = taskRef->rays;
		Renderer::Renderer* renderer = taskRef->renderer;

		Renderer::Frame* frame = renderer->GetRawFrame();
		int frameWidth = frame->GetWidth();
		int frameHeight = frame->GetHeight();

		// TODO: This logic should be done in the renderer 
		for (int rayIdx = startIdx; rayIdx < endIdx; rayIdx++) {
			const Renderer::RayMgr::Ray& ray = (*rays)[rayIdx];
			Util::Vector3 color = renderer->CalcTotalLight(ray);
			
			//! Store final color
			uint32_t colorAdj = (int)color.x << 6 * 4 | (int)color.y << 4 * 4 | (int)color.z << 2 * 4 | 0xFF;

			int pxBase = (rayIdx * resDownScale) % frameWidth;
			int pyBase = (rayIdx * resDownScale) / (frameWidth / resDownScale);

			for (int px = pxBase; px < pxBase + resDownScale && px < frameWidth; px++) { // Loop for downscaling
				for (int py = pyBase; py < pyBase + resDownScale && py < frameHeight; py++) {
					frame->SetPixel(px, py, colorAdj);
				}
			}
		}

		return true;
	}

}; // namespace Util
