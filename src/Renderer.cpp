//!
//! Renderer.cpp
//! Central component for rendering logic
//! 
#include "Renderer.h"



namespace Renderer {

	//! Constructor
	//! 
	Renderer::Renderer(const char* windowTitle, int windowWidth, int windowHeight) 
	: window("WindowFrame", windowWidth, windowHeight)
	, display(windowTitle, windowWidth, windowHeight)
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

	//! RenderFrameTest
	//! Debug test command
	//! 
	void Renderer::RenderFrameTest(Player::Player* player, int screenWidth, int screenHeight) {
		//! Retrieve camera information
		Player::Camera* camera = player->GetCamera();
		const Util::Vector3<double>& position = camera->GetPosition();
		const Util::Rotation& rotation = camera->GetRotation();
		double fov_rad = camera->GetFOV() * 180 / Util::PI;

		//! Get screen space information
		//double aspect_ratio = screenWidth / screenHeight;
		//double fovRad = fov * Util::PI / 180;



		/* ----------------------------------------------------------------
		 * Get camera FRU vector information
		 * ---------------------------------------------------------------- */

		// Forward vector
		Util::Vector3<double> camForward = camera->GetForwardVector();
		double roll = rotation.roll * Util::PI / 180;

		// World reference vector for RD vectors
		Util::Vector3<double> worldRefVec = Util::Vector3<double>::Up();
		if (std::abs(camForward.Dot(worldRefVec))) {
			worldRefVec = Util::Vector3<double>::Forward();
		}
		
		// Compute world axis orthogonal vectors
		Util::Vector3<double> camRightOrth = camForward.Cross(worldRefVec);
		Util::Vector3<double> camUpOrth = camRightOrth.Cross(camForward);
	

		// Roll camera RU vectors
		Util::Vector3<double> camRight = camRightOrth * std::cos(roll) +
										 camUpOrth * std::sin(roll);

		Util::Vector3<double> camUp = camUpOrth * std::cos(roll) -
									  camRightOrth * std::sin(roll);

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
				rays[rayIdx].vector = (x * camRight + y * camUp + camForward).Normalized();
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
			int py = rayI % screenWidth;

			//! Find first collision
			RayMgr::CollisionInfo* colInfo = RayMgr::GetFirstCollision(ray, nullptr);

			if (colInfo == nullptr) {
				this->window.SetPixel(px, py, 0xFF00FFFF);
				continue;
			}

			//! Handle collision
			const double reflectivity = 0.5;
			//colInfo->object
		}
	}

}; // namespace Renderer