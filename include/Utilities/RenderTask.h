//!
//! RenderTask.h
//! Defines a rendering task to be consumed and handled by a rendering thread
//! 
#pragma once

#include "WorkTask.h"
#include <vector>
#include "RayMgr.h"



namespace Util {

	class RenderTask : public WorkTask {
	public:
		int startIdx, endIdx;
		const std::vector<Renderer::RayMgr::Ray>* rays;

		RenderTask() 
			: rays()
			, startIdx(-1)
			, endIdx(-1)
		{}

		RenderTask(std::vector<Renderer::RayMgr::Ray>* rays, int startIdx, int endIdx)
			: rays(rays)
			, startIdx(startIdx)
			, endIdx(endIdx)
		{}
	};

}; // namespace Util