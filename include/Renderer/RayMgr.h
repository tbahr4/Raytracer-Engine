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

			//! Object entry collision
			Util::Vector3<double> position;
			Util::Vector3<double> normal;
			double distance;

			//! Object exit collision
			Util::Vector3<double> exitPosition;
			Util::Vector3<double> exitNormal;
			double exitDistance;

			CollisionInfo() : object(nullptr), position(), normal(), distance(0), exitPosition(), exitDistance(0) {}
		};

		std::unique_ptr<CollisionInfo> GetFirstCollision(World::World& world, const Ray& ray);
		std::unique_ptr<CollisionInfo> GetInternalCollision(World::Object& object, const Ray& ray);

		std::vector<RayMgr::Ray> GetDiffuseRays(const RayMgr::CollisionInfo* colInfo);
		RayMgr::Ray GetReflectionRay(const RayMgr::Ray& ray, const RayMgr::CollisionInfo* colInfo);
		RayMgr::Ray GetRefractionRay(const RayMgr::Ray& ray, const RayMgr::CollisionInfo* colInfo);

	}; // namespace RayMgr

}; // namespace Renderer
