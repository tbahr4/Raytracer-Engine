//!
//! RayMgr.cpp
//! Handles ray tracing and collision logic
//! 
#include "RayMgr.h"



namespace Renderer {

	namespace RayMgr {

		CollisionInfo* GetFirstCollision(World::World& world, const Ray& ray, World::Object* ignoreObj) {
			//! Perform collision logic for all objects by default
			int nObjects = world.GetObjectCount();
			for (int objI = 0; objI < nObjects; objI++) {
				World::Object* object = world.GetObject(objI);

				if (object == nullptr || object == ignoreObj) {
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
						double distance = (-b - std::sqrt(discriminant)) / (2 * a);

						if (distance >= 0) {	// Ignore collisions behind ray origin
							CollisionInfo* collision = new CollisionInfo();
							collision->distance = distance;
							collision->object = object;
							collision->position = ray.origin + ray.direction * distance;
							collision->normal = (collision->position - sphereCenter).Normalized();
							return collision;
						}
					}

					break;
				}
			}

			//! No collision found
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
