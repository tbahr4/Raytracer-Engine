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
					Util::Log::Error("GetFirstCollision: Unimplemented object shape defined for collision check");
					return nullptr;

				case World::ShapeType::SPHERE:
					
					Util::Vector3<double> sphereCenter = object->GetPosition();
					double sphereRadius = 1;	// FIXME: Need children types of shape object
					// TODO: Add rotation, scale of objects (sphere rotation does not matter)

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

							collision->object = object;

							//! Populate entry collision
							collision->distance = distance;
							collision->position = ray.origin + ray.direction * distance;
							collision->normal = (collision->position - sphereCenter).Normalized();

							//! Populate exit collision (identical to entry if minPosRootIdx is 1)
							collision->exitDistance = roots[1];
							collision->exitPosition = ray.origin + ray.direction * collision->exitDistance;
							collision->exitNormal = (collision->exitPosition - sphereCenter).Normalized();
						}
					}

					break;
				}
			}

			//! No collision found
			return collision;
		}

		std::unique_ptr<CollisionInfo> GetInternalCollision(World::Object& object, const Ray& ray) {
			// TODO: Both collision functions can share an internal collision function

			// Maintain the shortest distance collision
			std::unique_ptr<CollisionInfo> collision = nullptr;

			//! Handle collision depending on object type
			switch (object.GetShapeType()) {
			case World::ShapeType::CUBE:
			case World::ShapeType::RECTANGLE:
			default:
				Util::Log::Error("GetFirstCollision: Unimplemented object shape defined for collision check");
				return nullptr;

			case World::ShapeType::SPHERE:

				Util::Vector3<double> sphereCenter = object.GetPosition();
				double sphereRadius = 1;	// FIXME: Need children types of shape object
				// TODO: Add rotation, scale of objects (sphere rotation does not matter)

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
						return nullptr;	// No collision
					}

					double distance = roots[minPosRootIdx];

					if (distance >= 0) {	// Ignore collisions behind ray origin
						if (collision == nullptr) {
							collision = std::make_unique<CollisionInfo>();
						}

						collision->object = &object;

						//! Populate entry collision
						collision->distance = distance;
						collision->position = ray.origin + ray.direction * distance;
						collision->normal = (collision->position - sphereCenter).Normalized();

						// TODO: Clarity, these should always be identical in this function
						//! Populate exit collision (identical to entry if minPosRootIdx is 1)
						collision->exitDistance = roots[1];
						collision->exitPosition = ray.origin + ray.direction * collision->exitDistance;
						collision->exitNormal = (collision->exitPosition - sphereCenter).Normalized();
					}
				}

			}

			return collision;
		}

		//! GetDiffuseRays
		//! Returns the list of rays used to calculate diffuse light
		//! 
		std::vector<RayMgr::Ray> RayMgr::GetDiffuseRays(const RayMgr::CollisionInfo* colInfo) {
			if (colInfo == nullptr) {
				Util::Log::Error("GetDiffuseRays: Cannot create diffuse rays from null collision");
				return std::vector<RayMgr::Ray>();
			}

			// FIXME: Make this work in a loop of all lights
			const Util::Vector3<double> lightPos = { 0,5,-3 };
			RayMgr::Ray diffuseRay;
			diffuseRay.origin = colInfo->position;
			diffuseRay.direction = (lightPos - diffuseRay.origin).Normalized();

			//! Construct rays
			std::vector<RayMgr::Ray> rays{ diffuseRay };
			return rays;
		}

		RayMgr::Ray RayMgr::GetReflectionRay(const RayMgr::Ray& ray, const RayMgr::CollisionInfo* colInfo) {
			if (colInfo == nullptr) {
				Util::Log::Error("GetReflectionRay: Cannot create reflection ray from null collision");
				return RayMgr::Ray();
			}

			//! Construct ray
			RayMgr::Ray reflRay;
			reflRay.origin = colInfo->position;
			reflRay.direction = ray.direction - 2 * (ray.direction.Dot(colInfo->normal)) * colInfo->normal;
			return reflRay;
		}

		RayMgr::Ray RayMgr::GetRefractionRay(const RayMgr::Ray& ray, const RayMgr::CollisionInfo* colInfo) {
			if (colInfo == nullptr) {
				Util::Log::Error("GetRefractionRay: Cannot create refraction ray from null collision");
				return RayMgr::Ray();
			}

			// TODO: Functionize
			/* ----------------------------------------------------------------
			* Apply refraction via Snell's law
			* ---------------------------------------------------------------- */
			const double n1 = 1;		// TODO: Add to material mgr?
			const double n2 = 1.1;		// TODO: Add to material

			//! Determine refracted entry vector
			//! 
			double eta = n1 / n2;	// Refractive index ratio
			double cosI = ray.direction.Reversed().Dot(colInfo->normal);
			double sinT2 = eta * eta * (1.0 - cosI * cosI);	// Sin^2(theta_t)

			//! Assumption: Entry always occurs from air -> TIR can never occur
			// TODO: Remove me
			//if (sinT2 > 1) {
			//	// Total internal reflection - No refraction occurs
			//	Util::Log::WarnING("GetRefractionRay: Unimplemented reflection");
			//	return RayMgr::Ray(); // TODO: Handle internal reflection
			//}

			double cosT = std::sqrt(1 - sinT2);	// Cosine of transmitted angle

			Util::Vector3<double> entryDir = (eta * ray.direction + (eta * cosI - cosT) * colInfo->normal).Normalized();


			//! Determine refracted exit vector
			//! 
			eta = n2 / n1;	// Inverse
			cosI = entryDir.Dot(colInfo->exitNormal);
			sinT2 = eta * eta * (1.0 - cosI * cosI);	// Sin^2(theta_t)

			//if (sinT2 > 1) {
			//	// Total internal reflection - No refraction occurs
			//	Util::Log::WarnING("GetRefractionRay: Unimplemented reflection");
			//	return RayMgr::Ray(); // TODO: Handle internal reflection
			//}

			const int maxTIRRayDepth = 5;	// TODO: Configurable
			int rayDepth = 1;

			//! Determine if total internal reflection has occurred
			if (sinT2 > 1) {
				//! Create internal ray
				RayMgr::Ray internalRay;
				internalRay.direction = entryDir;
				internalRay.origin = colInfo->position;

				while (sinT2 > 1) {
					// Handle total internal reflection
					RayMgr::Ray reflRay = GetReflectionRay(internalRay, colInfo);
					std::unique_ptr<CollisionInfo> internalCol = GetInternalCollision(*(colInfo->object), internalRay);
					
					if (internalCol == nullptr) {
						Util::Log::Error("GetRefractionRay: Internal collision not found");
						return RayMgr::Ray();	// FIXME: Need better return handling
					}

					// Check for exit
					cosI = entryDir.Dot(internalCol->exitNormal);
					sinT2 = eta * eta * (1.0 - cosI * cosI);	// Sin^2(theta_t)

					// Construct ray - TODO: Clarity, this works but is not an entry
					if (sinT2 <= 1) {
						cosT = std::sqrt(1 - sinT2);	// Cosine of transmitted angle
						Util::Vector3<double> exitDir = (eta * internalRay.direction + (eta * cosI - cosT) * internalCol->exitNormal).Normalized();

						RayMgr::Ray refrRay;
						refrRay.origin = internalCol->exitPosition;
						refrRay.direction = exitDir;

						return refrRay;
					}






					rayDepth++;
					if (rayDepth == maxTIRRayDepth) {
						Util::Log::Warn("GetRefractionRay: Reached max TIR depth");
						return RayMgr::Ray();	// FIXME: Need to flag as complete and add to total ray depth
					}
				}
			}


			cosT = std::sqrt(1 - sinT2);	// Cosine of transmitted angle

			Util::Vector3<double> exitDir = (eta * entryDir + (eta * cosI - cosT) * colInfo->exitNormal).Normalized();


			// TODO: Calculate total transmission using Beer-Lambert law (per RGB component)

			/* ----------------------------------------------------------------
			* Construct ray
			* ---------------------------------------------------------------- */
			RayMgr::Ray refrRay;
			refrRay.origin = colInfo->exitPosition;
			refrRay.direction = exitDir;
			return refrRay;
		}

	}; // namespace RayMgr

}; // namespace Renderer
