//!
//! RenderTask.h
//! Defines a rendering task to be consumed and handled by a rendering thread
//! 
#pragma once

#include "WorkTask.h"
#include <vector>
#include "RayMgr.h"
#include "Renderer.h"



namespace Util {

	class RenderTask : public WorkTask {
	public:
		int startIdx, endIdx;
		const std::vector<Renderer::RayMgr::Ray>* rays;
		Renderer::Renderer* renderer;

		RenderTask() 
			: rays()
			, startIdx(-1)
			, endIdx(-1)
			, renderer(nullptr)
		{}

		RenderTask(std::vector<Renderer::RayMgr::Ray>* rays, int startIdx, int endIdx, Renderer::Renderer* renderer)
			: rays(rays)
			, startIdx(startIdx)
			, endIdx(endIdx)
			, renderer(renderer)
		{}
	};

}; // namespace Util