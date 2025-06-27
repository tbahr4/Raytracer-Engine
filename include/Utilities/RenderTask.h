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
		int rayVecOffset;
		std::vector<Renderer::RayMgr::Ray> rays;

		RenderTask(int rayVecOffset, std::vector<Renderer::RayMgr::Ray> rays)
			: rayVecOffset(rayVecOffset)
			, rays(rays)
		{}
	};

}; // namespace Util