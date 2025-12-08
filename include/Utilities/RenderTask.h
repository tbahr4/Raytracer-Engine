//!
//! RenderTask.h
//! Defines a rendering task to be consumed and handled by a rendering thread
//! 
#pragma once

#include "WorkTask.h"
#include <vector>
#include "RayMgr.h"
#include "Frame.h"
namespace Renderer {
	class Renderer;
}



namespace Util {

	class RenderTask : public WorkTask {
	public:
		int startIdx, endIdx;
		const std::vector<Renderer::RayMgr::Ray>* rays;
		std::shared_ptr<Renderer::FrameContext> frameCtx;
		Renderer::Renderer* renderer;

		RenderTask();
		RenderTask(std::vector<Renderer::RayMgr::Ray>* rays, int startIdx, int endIdx, std::shared_ptr<Renderer::FrameContext> frameCtx, Renderer::Renderer* renderer);
	};

}; // namespace Util