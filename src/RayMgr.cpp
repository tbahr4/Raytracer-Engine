//!
//! RayMgr.cpp
//! Handles ray tracing and collision logic
//! 
#include "RayMgr.h"



namespace Renderer {

	namespace RayMgr {

		CollisionInfo* GetFirstCollision(const Ray& ray, World::Object* ignoreObj) {
			return nullptr;
		}

		Ray* RayMgr::GetDiffuseRay(Ray* initialRay, CollisionInfo* colInfo) {
			return nullptr;
		}

		Ray* RayMgr::GetSpecularRay(Ray* initialRay, CollisionInfo* colInfo) {
			return nullptr;
		}

		Ray* RayMgr::GetRefractionRay(Ray* initialRay, CollisionInfo* colInfo) {
			return nullptr;
		}

		Util::Vector3<double> RayMgr::CalcIncidentLight(const Util::Vector3<double>& pointLightPos, const Util::Vector3<double>& pos) {
			return Util::Vector3<double>(0, 0, 0);
		}

	}; // namespace RayMgr

}; // namespace Renderer
