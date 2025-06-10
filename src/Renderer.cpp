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

	//! SetupFrameTest
	//! Debug test command
	//! 
	void Renderer::SetupFrameTest() {
		if (!display.IsActive()) return;

		for (int x=0;x<100;x++)
			for (int y=0;y<100;y++)
				this->window.SetPixel(x+280,y+200, 0xFF0000FF);

		display.PollEvents();
		display.RenderFrame(this->window);
		SDL_Delay(1 / 60);
	}

	//! RenderFrame
	//! Renders a single frame to the screen
	//! 
	void Renderer::RenderFrame(Player::Player* player) {
		/* ----------------------------------------------------------------
		 * Generate rays from given screen frame
		 * ---------------------------------------------------------------- */
		std::vector<RayMgr::Ray> rays = GenerateRays(player->GetCamera(), display.GetWidth(), display.GetHeight());

		/*
		
			foreach ray
				vec3 light = CalcTotalLight


			CalcTotalLight(maxDepth):
				_CalcTotalLightHelper(depth=0, maxDepth=N)		# max depth is total depth (reflect, refract, reflect = 3, not 2 and 1) (still a lot of iterations...)

			_CalcTotalLightHelper(ray, depth, maxDepth):		
				if (depth > maxDepth) {
					return {0,0,0};
				}

				col = GetFirstCollision()

				// Get rays
				rayDiffuse = GetDiffuseRay()
				rayRefl = GetReflectionRay()
				rayRefr = GetRefractionRay()

				// Get light components
				pDiffuse = X (diffuse)
				pReflect = Y (reflectivity)
				pRefract = Z (transparency)

				// Get component light
				cDiffuse = *** easy calc ***
				cReflection = _CalcTotalLightHelper(rayRefl, depth + 1, maxDepth)
				cRefraction = _CalcTotalLightHelper(rayRefr, depth + 1, maxDepth)

				// Handle totals
				tot = cDiffuse * pDiffuse + cReflect * pReflect + cRefract * pRefract
		
		*/
	}

	//! RenderFrameTest
	//! Debug test command
	//! 
	void Renderer::RenderFrameTest(Player::Player* player, int screenWidth, int screenHeight) {
		//! Retrieve camera information
		Player::Camera* camera = player->GetCamera();
		const Util::Vector3<double>& position = camera->GetPosition();
		const Util::Rotation& rotation = camera->GetRotation();
		double fov_rad = camera->GetFOV() * Util::PI / 180;

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
		std::vector<RayMgr::Ray> rays(screenWidth * screenHeight);

		double halfWidth = tan(fov_rad / 2);
		double aspectRatio = screenWidth / screenHeight;
		double halfHeight = halfWidth / aspectRatio;

		int rayIdx = 0;
		for (int py = 0; py < screenHeight; py++) {
			for (int px = 0; px < screenWidth; px++) {
				//! Normalize pixels to UV [-1,1]
				double u = ((px + 0.5) / screenWidth) * 2 - 1;
				double v = ((py + 0.5) / screenHeight) * 2 - 1;

				//! Scale UV by half the screen size
				double x = u * halfWidth;
				double y = v * halfHeight;

				//! Construct the ray
				rays[rayIdx].origin = position;
				rays[rayIdx].direction = (x * camRight + y * camUp + camForward).Normalized();
				rayIdx++;
			}
		}

		/* ----------------------------------------------------------------
		 * Perform collision logic
		 * ---------------------------------------------------------------- */
		for (int rayI = 0; rayI < rays.size(); rayI++) {
			
			//! Retrieve ray context
			const RayMgr::Ray& ray = rays[rayI];
			int px = rayI % screenWidth;
			int py = rayI / screenWidth;

			//! Find first collision
			RayMgr::CollisionInfo* firstCollision = RayMgr::GetFirstCollision(*world, ray, nullptr);

			if (firstCollision == nullptr) {
				this->window.SetPixel(px, py, 0x333333FF);
				continue;
			}

			//! Handle collision
			// diffuse + reflectance + transparency = 1.0
			double reflectivity = firstCollision->object->GetMaterial().reflectivity;
			double transparency = firstCollision->object->GetMaterial().transparency;
			double diffuse = 1 - reflectivity - transparency;

			if (diffuse < 0) {
				LOG_ERROR("Invalid diffuse/reflectivity/transparency value. Values must add up to 1");
			}

			Util::Vector3<double> cDiffuse;
			Util::Vector3<double> cRefl;
			Util::Vector3<double> cRefr;

			/* ----------------------------------------------------------------
			* Handle diffuse
			* ---------------------------------------------------------------- */
			const Util::Vector3<double> lightPos = { 0,5,3 };

			if (diffuse > 0) {
				RayMgr::Ray diffuseRay;
				diffuseRay.origin = firstCollision->position;
				diffuseRay.direction = (lightPos - diffuseRay.origin).Normalized();

				//! Color material if light is reached
				RayMgr::CollisionInfo* diffuseCol = RayMgr::GetFirstCollision(*world, diffuseRay, nullptr); // need to allow self collision
				if (diffuseCol == nullptr) {	// Bad check, need to check if light is collided with (in case something is behind the light)
					//! Calculate intensity
					double intensity = firstCollision->normal.Dot(diffuseRay.direction);

					//! Calculate color
					cDiffuse = firstCollision->object->GetMaterial().color * intensity;
				}
				else {
					cDiffuse = { 0,0,0 };
					delete diffuseCol;
				}
			}
			else {
				cDiffuse = { 0,0,0 };
			}

			/* ----------------------------------------------------------------
			* Handle spectral reflection
			* ---------------------------------------------------------------- */
			if (reflectivity > 0) {
				//! Determine reflection ray
				RayMgr::Ray reflRay;
				reflRay.origin = firstCollision->position;
				reflRay.direction = ray.direction - 2 * (ray.direction.Dot(firstCollision->normal)) * firstCollision->normal;

				// TODO: This should be recursive with max depth

				//! Find first collision
				RayMgr::CollisionInfo* reflCollision = RayMgr::GetFirstCollision(*world, reflRay, nullptr);

				if (reflCollision == nullptr) {
					cRefl = { 0,0,0 };
				}
				else {
					//! TODO: This should be diffuse (and rest of light)
					cRefl = reflCollision->object->GetMaterial().color;
					delete reflCollision;
				}
			}
			else {
				cRefl = { 0,0,0 };
			}

			/* ----------------------------------------------------------------
			* Handle refraction
			* ---------------------------------------------------------------- */
			if (transparency > 0) {
				//! Determine refraction ray
				// TODO: Refract
				RayMgr::Ray refrRay;
				refrRay.origin = firstCollision->position;
				refrRay.direction = ray.direction;

				// TODO: This should be recursive with max depth

				//! Find first collision
				RayMgr::CollisionInfo* reflCollision = RayMgr::GetFirstCollision(*world, refrRay, firstCollision->object);

				if (reflCollision == nullptr) {
					cRefl = { 0,0,0 };
				}
				else {
					//! TODO: This should be diffuse (and rest of light)
					cRefl = reflCollision->object->GetMaterial().color;
					delete reflCollision;
				}
			}
			else {
				cRefr = { 0,0,0 };
			}
			
			/* ----------------------------------------------------------------
			* Obtain total light value
			* ---------------------------------------------------------------- */
			Util::Vector3 totalColor = (cDiffuse * diffuse) + (cRefl * reflectivity) + (cRefr * transparency);
			int colorAdj = (int)totalColor.x << 6 * 4 | (int)totalColor.y << 4 * 4 | (int)totalColor.z << 2 * 4 | 0xFF;
			this->window.SetPixel(px, py, colorAdj);

			delete firstCollision;
		}

		/* ----------------------------------------------------------------
		 * Display Driver logic
		 * ---------------------------------------------------------------- */
		display.PollEvents();
		inputMgr->ProcessActivityState();	// Process valid activities
		display.RenderFrame(this->window);
		SDL_Delay(1 / 60);
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