//!
//! RenderTask.cpp
//! Defines a rendering task to be consumed and handled by a rendering thread
//! 
#include "RenderTask.h"
#include "Renderer.h"


namespace Util {

	RenderTask::RenderTask()
		: rays()
		, startIdx(-1)
		, endIdx(-1)
		, renderer(nullptr)
	{}

	RenderTask::RenderTask(std::vector<Renderer::RayMgr::Ray>* rays, int startIdx, int endIdx, std::shared_ptr<Renderer::FrameContext> frameCtx, Renderer::Renderer* renderer)
		: rays(rays)
		, startIdx(startIdx)
		, endIdx(endIdx)
		, frameCtx(frameCtx)
		, renderer(renderer)
	{}

}; // namespace Util