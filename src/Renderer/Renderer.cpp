//!
//! Renderer.cpp
//! Central component for rendering logic
//! 
#include "Renderer.h"
#include "Engine.h"
using namespace Engine;



namespace Renderer {

	//! Constructor
	//! 
	Renderer::Renderer(const char* windowTitle, int windowWidth, int windowHeight, std::shared_ptr<Player::Player> player, std::shared_ptr<World::World> world, std::shared_ptr<InputMgr::InputMgr> inputMgr, int maxRayDepth, int resDownScale)
	: window("WindowFrame", 0, 0, windowWidth, windowHeight)
	, display(windowTitle, windowWidth, windowHeight, player, world, inputMgr)
	, world(world)
	, inputMgr(inputMgr)
	, maxRayDepth(maxRayDepth)
	, resDownScale(resDownScale)
	, skybox(std::make_unique<World::Skybox>())
	, renderPool("RenderPool", Config::NUM_RENDER_THREADS, Config::RESOLUTION_DOWN_SCALE)
	, renderTasks(std::ceil((Config::SCREEN_WIDTH* Config::SCREEN_HEIGHT / (Config::RESOLUTION_DOWN_SCALE * Config::RESOLUTION_DOWN_SCALE)) / (double)Config::NUM_RAYS_PER_TASK))
	{
		for (int i = 0; i < renderTasks.size(); i++) {
			renderTasks[i] = std::make_shared<Util::RenderTask>();
		}
	}

	//! Destructor
	//! 
	Renderer::~Renderer() {
		renderPool.Shutdown();
	}

	//! Init
	//! Initializes the renderer to an active state
	//! 
	bool Renderer::Init() {
		bool success = this->display.Init();

		if (!success) {
			Util::Log::Error("Renderer initialization failed");
			return false;
		}

#ifndef SINGLE_THREADED
		renderPool.Init();
		Util::Log::Info("Render pool initialized");
#endif

		Util::Log::Info("Renderer initialized successfully");
		this->isInitialized = true;
		return true;
	}

	//! IsActive
	//! Returns the activity state of the renderer. Establishes readiness for processing commands
	//! 
	bool Renderer::IsActive() const {
		return this->isInitialized && display.IsActive();
	}

	//! ProduceFrame
	//! Populates frame data and stores within internal buffers for later rendering
	//! 
	void Renderer::ProduceFrame(std::shared_ptr<FrameContext> frameCtx) {
		/* ----------------------------------------------------------------
		 * Generate rays from given screen frame
		 * ---------------------------------------------------------------- */
		int frameWidth = frameCtx->frame->GetWidth();
		int frameHeight = frameCtx->frame->GetHeight();
		std::vector<RayMgr::Ray> rays = GenerateRays(frameCtx->camera, frameWidth, frameHeight);

		/* ----------------------------------------------------------------
		 * Calculate total light for each ray
		 * ---------------------------------------------------------------- */

#ifdef SINGLE_THREADED
		for (int rayIdx = 0; rayIdx < rays.size(); rayIdx++) {
			RayMgr::Ray& ray = rays[rayIdx];
			Util::Vector3 color = CalcTotalLight(ray);

			//! Set the frame's pixel
			uint32_t colorAdj = (int)color.x << 6 * 4 | (int)color.y << 4 * 4 | (int)color.z << 2 * 4 | 0xFF;

			int pxBase = (rayIdx * resDownScale) % frameWidth;
			int pyBase = (rayIdx * resDownScale) / (frameWidth / resDownScale);

			for (int px = pxBase; px < pxBase + resDownScale && px < frameWidth; px++) { // Loop for downscaling
				for (int py = pyBase; py < pyBase + resDownScale && py < frameHeight; py++) {
					frameCtx->frame->SetPixel(px, py, colorAdj);
				}
			}
		}
#else
 		//! Split into rendering tasks
 		int nTasks = std::ceil(rays.size() / (double)Config::NUM_RAYS_PER_TASK);
 		assert(nTasks == tasks.size());
 
 		for (int taskI = 0; taskI < renderTasks.size(); taskI++) {
 			renderTasks[taskI]->GetNewUID();
 			renderTasks[taskI]->startIdx = taskI * Config::NUM_RAYS_PER_TASK;
 			renderTasks[taskI]->endIdx = std::min(renderTasks[taskI]->startIdx + Config::NUM_RAYS_PER_TASK, (int)rays.size());
 			renderTasks[taskI]->rays = &rays;
			renderTasks[taskI]->frameCtx = frameCtx;
 			renderTasks[taskI]->renderer = this;
 		}
 
 		//! Add tasks to render pool
 		Util::Log::Debug("Adding " + std::to_string(renderTasks.size()) + " tasks");
 		renderPool.AddTasks(renderTasks);
 
 		renderPool.WaitIdle();
#endif
	}

	//! AddFrame
	//! Adds a new rendering frame of given size at the specified position
	//! 
	void Renderer::AddFrame(std::string name, int x, int y, int sizeX, int sizeY, std::shared_ptr<Player::Camera>camera) {
		this->frames.push_back(std::make_shared<FrameContext>(std::make_shared<Frame>(name, x, y, sizeX, sizeY), camera));
	}

	//! RenderFrames
	//! Populates and renders all frames to the window frame
	//! 
	void Renderer::RenderFrames() {
		//! Populate the frames
		for (int frameI = 0; frameI < frames.size(); frameI++) {
			std::shared_ptr<FrameContext> frameCtx = frames[frameI];
			this->ProduceFrame(frameCtx);
		}

		//! Draw the frames to the window
		for (int frameI = 0; frameI < frames.size(); frameI++) {
			std::shared_ptr<FrameContext> frameCtx = frames[frameI];
			int maxOffsetX = std::min(frameCtx->frame->GetWidth(), window.GetWidth());
			int maxOffsetY = std::min(frameCtx->frame->GetHeight(), window.GetHeight());

			for (int offsetY = 0; offsetY < maxOffsetY; offsetY++) {
				for (int offsetX = 0; offsetX < maxOffsetX; offsetX++) {
					window.SetPixel(frameCtx->frame->GetPosX() + offsetX, frameCtx->frame->GetPosY() + offsetY, frameCtx->frame->GetPixel(offsetX, offsetY)); // TODO: For now, just overwrite previous frames, until transparency allows it (also need to skip if covered pixel?)
				}
			}
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
		SDL_Delay(1 / 360);
	}

	//! CalcTotalLight
	//! Returns the total resultant light provided by the given ray trace
	//! 
	Util::Vector3<double> Renderer::CalcTotalLight(const RayMgr::Ray& ray) const {
		return _CalcTotalLightHelper(ray, 0);
	}

	//! GetRawWindowFrame
	//! Returns the raw window frame for external modification
	//! 
	Frame* Renderer::GetRawWindowFrame() {
		return &window;
	}

	//! _CalcTotalLightHelper
	//! Helper function for CalcTotalLight
	//! Note: This function is for primary rays only (e.g., collisions return the color) instead of shadow rays (collisions return black).
	//!       Shadow rays (from diffuse calculations) do not recurse
	//! 
	Util::Vector3<double> Renderer::_CalcTotalLightHelper(const RayMgr::Ray& ray, int depth) const {
		//! Base case
		if (depth > maxRayDepth) {
			return GetSkyboxColor(ray);
		}

		//! Fire ray
		std::unique_ptr<RayMgr::CollisionInfo> firstCol = RayMgr::GetFirstCollision(*world, ray);

		if (firstCol == nullptr) {
			return GetSkyboxColor(ray);
		}

		//! Get percentage contribution of each ray component
		double pctRefl = firstCol->object->GetMaterial().reflectivity;
		double pctRefr = firstCol->object->GetMaterial().transparency;
		double pctDiff = 1 - pctRefl - pctRefr;
		assert(pctDiff + pctRefl + pctRefr == 1);

		//! Calculate diffuse component
		Util::Vector3<double> colDiff = { 0,0,0 };
		if (pctDiff > 0) {
			std::vector<RayMgr::Ray> rayDiffs = GetDiffuseRays(firstCol.get());

			Util::Vector3<double> totLight = { 0,0,0 };
			for (int rayI = 0; rayI < rayDiffs.size(); rayI++) {
				//! Get collision
				auto diffCollisions = RayMgr::GetAllCollisions(*world, rayDiffs[rayI]);

				//! Determine loss due to object collision opacity
				double opacityLossMult = 1;
				for (int colI = 0; colI < diffCollisions.size(); colI++) {
					auto col = diffCollisions[colI].get();
					opacityLossMult *= col->object->GetMaterial().transparency;

					if (opacityLossMult == 0) {
						break;
					}
				}

				//! Determine light of component
				double intensity = std::max(0.0, firstCol->normal.Dot(rayDiffs[rayI].direction)) * opacityLossMult; // TODO: HDR
				const Util::Vector3<double> lightColor = (rayI == 0 ? Config::LIGHT_COLOR : Util::Vector3<double>{0,0,150}) / 255; // TODO: Create light object as renderable
				totLight += lightColor * intensity;
			}

			//! Determine final color
			totLight.x = std::min(1.0, totLight.x);
			totLight.y = std::min(1.0, totLight.y);
			totLight.z = std::min(1.0, totLight.z);
			colDiff = firstCol->object->GetMaterial().color * totLight;
		}

		//! Calculate reflection component
		Util::Vector3<double> colRefl = { 0,0,0 };
		if (pctRefl > 0) {
			RayMgr::Ray rayRefl = GetReflectionRay(ray, firstCol.get());
			colRefl = _CalcTotalLightHelper(rayRefl, depth + 1);
		}

		//! Calculate refraction component
		Util::Vector3<double> colRefr = { 0,0,0 };
		if (pctRefr > 0) {
			RayMgr::Ray rayRefr = GetRefractionRay(ray, firstCol.get());
			colRefr = _CalcTotalLightHelper(rayRefr, depth + 1);
		}

		//! Determine total resultant light
		return (colDiff * pctDiff) + (colRefl * pctRefl) + (colRefr * pctRefr);
	}

	//! GetSkyboxColor
	//! Returns the skybox color that results from the given ray
	//! 
	Util::Vector3<double> Renderer::GetSkyboxColor(const RayMgr::Ray& ray) const {
		//double normT = (ray.direction.y + 1) / 2;
		//return (1 - normT) * Config::FLOOR_COLOR + normT * Config::CEILING_COLOR;


		//! Get the side
		World::Skybox::Side side;

		double rayX = ray.direction.x;
		double rayY = ray.direction.y;
		double rayZ = ray.direction.z;

		double  absX = std::abs(rayX);
		double  absY = std::abs(rayY);
		double  absZ = std::abs(rayZ);

		if (absX >= absY && absX >= absZ) { // X is dominant
			if (rayX > 0) side = World::Skybox::Side::LEFT;
			else       side = World::Skybox::Side::RIGHT;
		}
		else if (absY >= absX && absY >= absZ) { // Y is dominant
			if (rayY > 0) side = World::Skybox::Side::UP;
			else       side = World::Skybox::Side::DOWN;
		}
		else { // Z is dominant
			if (rayZ > 0) side = World::Skybox::Side::FRONT;
			else       side = World::Skybox::Side::BACK;
		}

		//! Get UV coordinate
		double u, v;

		switch (side) {
		case World::Skybox::Side::RIGHT:  u = -rayZ / absX; v = -rayY / absX; break;
		case World::Skybox::Side::LEFT:   u = rayZ / absX; v = -rayY / absX; break;
		case World::Skybox::Side::UP:    u = rayX / absY; v = rayZ / absY; break;
		case World::Skybox::Side::DOWN: u = rayX / absY; v = -rayZ / absY; break;
		case World::Skybox::Side::FRONT:  u = rayX / absZ; v = -rayY / absZ; break;
		case World::Skybox::Side::BACK:   u = -rayX / absZ; v = -rayY / absZ; break;
		default: u = 0; v = 0; break;
		}

		u = 0.5 + (-u * .5);
		v = 0.5 + (v * .5);

		if (side == World::Skybox::Side::LEFT || side == World::Skybox::Side::RIGHT) {
			u = 1-u;
		}

		//! Determine pixel
		return skybox->GetPixel(u, v, side);
	}

	//! GenerateRays
	//! Generates a list of rays from the given camera properties and frame size
	//! 
	std::vector<RayMgr::Ray> Renderer::GenerateRays(std::shared_ptr<Player::Camera> camera, int frameWidth, int frameHeight) {
		/* ----------------------------------------------------------------
		 * Get camera FRU vector information
		 * ---------------------------------------------------------------- */
		const Player::Camera::FRUVector& fruVector = camera->GetFRUVector();
		const Util::Vector3<double>& camForward = fruVector.forward;
		const Util::Vector3<double>& camRight = fruVector.right;
		const Util::Vector3<double>& camUp = fruVector.up;

		/* ----------------------------------------------------------------
		 * Generate rays
		 * ---------------------------------------------------------------- */
		double raysX = std::ceil(frameWidth / resDownScale);
		double raysY = std::ceil(frameHeight / resDownScale);

		std::vector<RayMgr::Ray> rays(raysX * raysY);

		double halfWidth = tan((camera->GetFOV() * Util::PI / 180) / 2);
		double aspectRatio = static_cast<double>(frameWidth) / static_cast<double>(frameHeight);
		double halfHeight = halfWidth / aspectRatio;

		int rayIdx = 0;
		for (int py = 0; py < raysY; py++) {
			for (int px = 0; px < raysX; px++) {
				//! Normalize pixels to UV [-1,1]
				double u = (((px * resDownScale) + 0.5) / frameWidth) * 2 - 1;
				double v = (((py * resDownScale) + 0.5) / frameHeight) * 2 - 1;

				//! Scale UV by half the screen size
				double x = u * halfWidth;
				double y = v * halfHeight;

				//! Construct the ray
				rays[rayIdx].origin = camera->GetPosition();
				rays[rayIdx].direction = (x * camRight - y * camUp + camForward).Normalized();
				rayIdx++;
			}
		}

		return rays;
	}

	//! GetWindowWidth
	//! Returns the width of the window in pixels
	//! 
	int Renderer::GetWindowWidth() const {
		return window.GetWidth();
	}

	//! GetWindowHeight
	//! Returns the height of the window in pixels
	//! 
	int Renderer::GetWindowHeight() const {
		return window.GetHeight();
	}

}; // namespace Renderer