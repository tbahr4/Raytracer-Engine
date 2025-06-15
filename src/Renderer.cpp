//!
//! Renderer.cpp
//! Central component for rendering logic
//! 
#include "Renderer.h"



namespace Renderer {

	//! Constructor
	//! 
	Renderer::Renderer(const char* windowTitle, int windowWidth, int windowHeight, Player::Player* player, World::World* world, InputMgr::InputMgr* inputMgr)
	: window("WindowFrame", windowWidth, windowHeight)
	, display(windowTitle, windowWidth, windowHeight, player, world, inputMgr)
	, world(world)
	, inputMgr(inputMgr)
	{}

	//! Init
	//! Initializes the renderer to an active state
	//! 
	bool Renderer::Init() {
		bool success = this->display.Init();

		if (!success) {
			LOG_ERROR("Renderer initialization failed");
			return false;
		}

		LOG_INFO("Renderer initialized successfully");
		this->isInitialized = true;
		return true;
	}

	//! IsActive
	//! Returns the activity state of the renderer. Establishes readiness for processing commands
	//! 
	bool Renderer::IsActive() const {
		return this->isInitialized && display.IsActive();
	}

	//! ProduceWorldFrame
	//! Produces a world frame and stores within internal buffers for later rendering
	//! 
	void Renderer::ProduceWorldFrame(Player::Player* player) {
		// TODO: This can be decomposed more

		/* ----------------------------------------------------------------
		 * Generate rays from given screen frame
		 * ---------------------------------------------------------------- */
		std::vector<RayMgr::Ray> rays = GenerateRays(player->GetCamera(), display.GetWidth(), display.GetHeight());


		/* ----------------------------------------------------------------
		 * Calculate total light for each ray
		 * ---------------------------------------------------------------- */
		for (int rayIdx = 0; rayIdx < rays.size(); rayIdx++) {
			RayMgr::Ray& ray = rays[rayIdx];
			Util::Vector3 color = CalcTotalLight(ray, maxRayDepth);

			// Just set window pixel for now
			int colorAdj = (int)color.x << 6 * 4 | (int)color.y << 4 * 4 | (int)color.z << 2 * 4 | 0xFF;
			int px = rayIdx % display.GetWidth();
			int py = rayIdx / display.GetWidth();
			this->window.SetPixel(px, py, colorAdj);
		}

		


	}

	//! DisplayFrame
	//! Forwards the window frame in its current state to the display driver for rendering
	//! 
	void Renderer::DisplayFrame() {
		// TODO: Parameterize/Separate
		display.PollEvents();
		inputMgr->ProcessActivityState();	// Process valid activities
		display.RenderFrame(this->window);
		SDL_Delay(1 / 60);
	}

	//! CalcTotalLight
	//! Returns the total resultant light provided by the given ray trace
	//! 
	Util::Vector3<double> Renderer::CalcTotalLight(const RayMgr::Ray& ray, int maxRayDepth) {
		return _CalcTotalLightHelper(ray, 0, maxRayDepth);
	}

	//! _CalcTotalLightHelper
	//! Helper function for CalcTotalLight
	//! 
	Util::Vector3<double> Renderer::_CalcTotalLightHelper(const RayMgr::Ray& ray, int depth, int maxRayDepth) {
		//! Base case
		if (depth > maxRayDepth) {
			return { 0,0,0 };	// No light contribution
		}

		//! Get first collision
		RayMgr::CollisionInfo* firstCol = RayMgr::GetFirstCollision(*world, ray, nullptr);

		if (firstCol == nullptr) {
			// No further contribution
			return { 0,0,0 };
		}
		
		//! Get object's light properties
		double pctRefl = firstCol->object->GetMaterial().reflectivity;
		double pctRefr = firstCol->object->GetMaterial().transparency;
		double pctDiff = 1 - pctRefl - pctRefr;

		if (pctDiff < 0) {
			LOG_ERROR("Renderer: Invalid object properties. Sum of reflectivity and transparency must be at most 1.0");
			delete firstCol;
			return { 0,0,0 };
		}

		//! Get coincident rays
		std::vector<RayMgr::Ray> rayDiffs = GetDiffuseRays(firstCol);  
		RayMgr::Ray rayRefl = GetReflectionRay(ray, firstCol);
		RayMgr::Ray rayRefr = GetRefractionRay(ray, firstCol);

		// TODO: return early if max depth
		// TODO: only spawn ray if light property allows it

		/* ----------------------------------------------------------------
		 * Get component light
		 * ---------------------------------------------------------------- */
		//! Diffuse
		// TODO: functionize this
		std::vector<Util::Vector3<double>> diffuseComps(rayDiffs.size());
		for (int lightI = 0; lightI < diffuseComps.size(); lightI++) {
			//! Calculate diffuse due to given light
			const RayMgr::Ray& diffuseRay = rayDiffs[lightI];

			//! Color material if light is reached
			RayMgr::CollisionInfo* diffuseCol = RayMgr::GetFirstCollision(*world, diffuseRay, nullptr);
			if (diffuseCol == nullptr) {	// TODO: Bad check, need to check if light is collided with (in case something is behind the light). Make light an object to make collision info check simply "isLight?"
				// Not obscured by an object before reaching light
				// FIXME: Diffuse collisions with transparent objects allows light to pass through

				//! Calculate intensity
				double intensity = std::max(0.0, firstCol->normal.Dot(diffuseRay.direction));

				// TODO: Calculate light falloff
				// TODO: add light color

				//! Calculate color
				diffuseComps[lightI] = firstCol->object->GetMaterial().color * intensity;
			}
			else {
				diffuseComps[lightI] = { 0,0,0 };
				delete diffuseCol;
			}
		}

		delete firstCol;
		
		// Sum diffuse light contributions
		// TODO: add HDR rendering for exceeding 255 intensity
		Util::Vector3<double> colDiff = { 0,0,0 };
		for (int lightI = 0; lightI < diffuseComps.size(); lightI++) {
			colDiff = colDiff + diffuseComps[lightI];
		}

		//! Reflection and refraction
		Util::Vector3<double> colRefl = _CalcTotalLightHelper(rayRefl, depth + 1, maxRayDepth);
		Util::Vector3<double> colRefr = _CalcTotalLightHelper(rayRefr, depth + 1, maxRayDepth);
		
		/* ----------------------------------------------------------------
		 * Get total light
		 * ---------------------------------------------------------------- */
		Util::Vector3<double> totalLight = (colDiff * pctDiff) + (colRefl * pctRefl) + (colRefr * pctRefr);
		
		return totalLight;
	}

	//! GetDiffuseRays
	//! Returns the list of rays used to calculate diffuse light
	//! 
	std::vector<RayMgr::Ray> Renderer::GetDiffuseRays(RayMgr::CollisionInfo* colInfo) {		
		// FIXME: Make this work in a loop of all lights
		const Util::Vector3<double> lightPos = { 0,5,3 };
		RayMgr::Ray diffuseRay;
		diffuseRay.origin = colInfo->position;
		diffuseRay.direction = (lightPos - diffuseRay.origin).Normalized();

		std::vector<RayMgr::Ray> rays{ diffuseRay };
		return rays;
	}

	RayMgr::Ray Renderer::GetReflectionRay(const RayMgr::Ray& ray, RayMgr::CollisionInfo* colInfo) {
		// FIXME: check collision null
		RayMgr::Ray reflRay;
		reflRay.origin = colInfo->position;
		reflRay.direction = ray.direction - 2 * (ray.direction.Dot(colInfo->normal)) * colInfo->normal;
		return reflRay;
	}

	RayMgr::Ray Renderer::GetRefractionRay(const RayMgr::Ray& ray, RayMgr::CollisionInfo* colInfo) {
		// FIXME: check collision null
		// FIXME: Ray should start after collision
		// TODO: Apply refraction
		RayMgr::Ray refrRay;
		refrRay.origin = colInfo->exitPosition;
		refrRay.direction = ray.direction;
		return refrRay;
	}

	//! GenerateRays
	//! Generates a list of rays from the given camera properties and frame size
	//! 
	std::vector<RayMgr::Ray> Renderer::GenerateRays(Player::Camera* camera, int frameWidth, int frameHeight) {
		/* ----------------------------------------------------------------
		 * Get camera FRU vector information
		 * ---------------------------------------------------------------- */
		Player::Camera::FRUVector& fruVector = camera->GetFRUVector();
		Util::Vector3<double>& camForward = fruVector.forward;
		Util::Vector3<double>& camRight = fruVector.right;
		Util::Vector3<double>& camUp = fruVector.up;

		/* ----------------------------------------------------------------
		 * Generate rays
		 * ---------------------------------------------------------------- */
		std::vector<RayMgr::Ray> rays(frameWidth * frameHeight);

		double halfWidth = tan((camera->GetFOV() * Util::PI / 180) / 2);
		double aspectRatio = frameWidth / frameHeight;
		double halfHeight = halfWidth / aspectRatio;

		int rayIdx = 0;
		for (int py = 0; py < frameHeight; py++) {
			for (int px = 0; px < frameWidth; px++) {
				//! Normalize pixels to UV [-1,1]
				double u = ((px + 0.5) / frameWidth) * 2 - 1;
				double v = ((py + 0.5) / frameHeight) * 2 - 1;

				//! Scale UV by half the screen size
				double x = u * halfWidth;
				double y = v * halfHeight;

				//! Construct the ray
				rays[rayIdx].origin = camera->GetPosition();
				rays[rayIdx].direction = (x * camRight + y * camUp + camForward).Normalized();
				rayIdx++;
			}
		}

		return rays;
	}

}; // namespace Renderer