//!
//! RayMgr.h
//! Handles ray tracing and collision logic
//! 
#pragma once

#include "Util.h"
#include "World.h"
#include "Object.h"



namespace Renderer {

	namespace RayMgr {

		struct Ray {
			Util::Vector3<double> origin;
			Util::Vector3<double> direction;
		};

		struct CollisionInfo {
			//! Primary collision
			World::Object* object;
			Util::Vector3<double> position;
			Util::Vector3<double> normal;
			double distance;

			//! Secondary collision (on exit)
			Util::Vector3<double> exitPosition;
			double exitDistance;

			CollisionInfo() : object(nullptr), position(), normal(), distance(0), exitPosition(), exitDistance(0) {}
		};

		std::unique_ptr<CollisionInfo> GetFirstCollision(World::World& world, const Ray& ray);

		std::vector<RayMgr::Ray> GetDiffuseRays(const RayMgr::CollisionInfo* colInfo);
		RayMgr::Ray GetReflectionRay(const RayMgr::Ray& ray, const RayMgr::CollisionInfo* colInfo);
		RayMgr::Ray GetRefractionRay(const RayMgr::Ray& ray, const RayMgr::CollisionInfo* colInfo);

	}; // namespace RayMgr

}; // namespace Renderer
