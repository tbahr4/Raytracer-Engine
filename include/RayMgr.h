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

		CollisionInfo* GetFirstCollision(World::World& world, const Ray& ray, World::Object* ignoreObj);

		Ray* GetDiffuseRay(Ray* initialRay, CollisionInfo* colInfo);
		Ray* GetSpecularRay(Ray* initialRay, CollisionInfo* colInfo);
		Ray* GetRefractionRay(Ray* initialRay, CollisionInfo* colInfo);

		Util::Vector3<double> CalcIncidentLight(const Util::Vector3<double>& pointLightPos, const Util::Vector3<double>& pos);

	}; // namespace RayMgr

}; // namespace Renderer
