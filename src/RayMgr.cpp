//!
//! RayMgr.cpp
//! Handles ray tracing and collision logic
//! 
#include "RayMgr.h"



namespace Renderer {

	namespace RayMgr {

		//! GetFirstCollision
		//! Returns the nearest object collision, if any, from the given ray
		//! Ignores the given object if specified
		//! 
		std::unique_ptr<CollisionInfo> GetFirstCollision(World::World& world, const Ray& ray) {
			// Maintain the shortest distance collision
			std::unique_ptr<CollisionInfo> collision = nullptr;
			double minDist = INFINITY;

			//! Perform collision logic for all objects by default
			int nObjects = world.GetObjectCount();
			for (int objI = 0; objI < nObjects; objI++) {
				World::Object* object = world.GetObject(objI);

				if (object == nullptr) {
					continue;
				}

				//! Handle collision depending on object type
				switch (object->GetShapeType()) {
				case World::ShapeType::CUBE:
				case World::ShapeType::RECTANGLE:
				default:
					LOG_ERROR("GetFirstCollision: Unimplemented object shape defined for collision check");
					return nullptr;

				case World::ShapeType::SPHERE:
					
					Util::Vector3<double> sphereCenter = object->GetPosition();
					double sphereRadius = 1;	// FIXME: Need children types of shape object

					Util::Vector3<double> offsetRayOrigin = ray.origin - sphereCenter;	// Offset ray as if sphere was at (0,0,0)

					// sqrLength(rayOrigin + rayDir * distance) = r^2
					// 
					double a = ray.direction.Dot(ray.direction);	// Should be 1
					double b = 2 * offsetRayOrigin.Dot(ray.direction);
					double c = offsetRayOrigin.Dot(offsetRayOrigin) - sphereRadius * sphereRadius;

					double discriminant = (b * b) - (4 * a * c);

					//! Check for collision
					//! discriminant < 0 -> miss
					if (discriminant >= 0) {
						double roots[] = { 
							(-b - std::sqrt(discriminant)) / (2 * a), 
							(-b + std::sqrt(discriminant)) / (2 * a)
						};

						//! Get index of smallest positive root
						int minPosRootIdx = (roots[0] > 0) ? 0 : (roots[1] > 0 ? 1 : -1);
						if (minPosRootIdx == -1) {
							continue;	// No collision
						}

						double distance = roots[minPosRootIdx];

						if (distance >= 1e-9 && distance < minDist) {	// Ignore collisions behind ray origin
							minDist = distance;
							if (collision == nullptr) {
								collision = std::make_unique<CollisionInfo>();
							}

							collision->distance = distance;
							collision->object = object;
							collision->position = ray.origin + ray.direction * distance;
							collision->normal = (collision->position - sphereCenter).Normalized();

							//! Handle exit collision (identical to entry if minPosRootIdx is 1)
							collision->exitDistance = roots[1];
							collision->exitPosition = ray.origin + ray.direction * collision->exitDistance;
						}
					}

					break;
				}
			}

			//! No collision found
			return collision;
		}

		//! GetDiffuseRays
		//! Returns the list of rays used to calculate diffuse light
		//! 
		std::vector<RayMgr::Ray> RayMgr::GetDiffuseRays(const RayMgr::CollisionInfo* colInfo) {
			// FIXME: Make this work in a loop of all lights
			const Util::Vector3<double> lightPos = { 0,5,3 };
			RayMgr::Ray diffuseRay;
			diffuseRay.origin = colInfo->position;
			diffuseRay.direction = (lightPos - diffuseRay.origin).Normalized();

			std::vector<RayMgr::Ray> rays{ diffuseRay };
			return rays;
		}

		RayMgr::Ray RayMgr::GetReflectionRay(const RayMgr::Ray& ray, const RayMgr::CollisionInfo* colInfo) {
			// FIXME: check collision null
			RayMgr::Ray reflRay;
			reflRay.origin = colInfo->position;
			reflRay.direction = ray.direction - 2 * (ray.direction.Dot(colInfo->normal)) * colInfo->normal;
			return reflRay;
		}

		RayMgr::Ray RayMgr::GetRefractionRay(const RayMgr::Ray& ray, const RayMgr::CollisionInfo* colInfo) {
			// FIXME: check collision null
			// FIXME: Ray should start after collision
			// TODO: Apply refraction
			RayMgr::Ray refrRay;
			refrRay.origin = colInfo->exitPosition;
			refrRay.direction = ray.direction;
			return refrRay;
		}

	}; // namespace RayMgr

}; // namespace Renderer
