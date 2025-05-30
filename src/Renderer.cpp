//!
//! Renderer.cpp
//! Central component for rendering logic
//! 
#include "Renderer.h"
#include "Util.h"


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
		double fov = camera->GetFOV();

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
		 * Setup ray information
		 * ---------------------------------------------------------------- */
		std::vector<RayMgr::Ray> rays(123);

		// TODO: rotate by theta_x and theta_y
		// for theta_y = 0...
		// rotated_dir = cos(theta_x) dot camForward  +  sin(theta_x) dot camRight
		// - This gives the rotation from forward to right by theta_x degrees
		// - Will need to do generate all theta_y, then process all theta_x per theta_y, 

		// NOTE: Each ray should not be linearly spaced


		//Util::Vector3<double> camPlaneNormal = position

		//RayMgr::GetFirstCollision()
	}

}; // namespace Renderer