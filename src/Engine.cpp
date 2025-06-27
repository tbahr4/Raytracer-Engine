//!
//! Engine.cpp
//! Manages the raytracing engine
//! 
#include "Engine.h"



namespace Engine {

	//! Constructor
	//! 
	Engine::Engine(int nRenderThreads)
		: isActive(false)
		, nRenderThreads(nRenderThreads)
		, renderPool("RenderPool", nRenderThreads)
	{}

	//! Destructor
	//! 
	Engine::~Engine() {
		renderPool.Shutdown();
	}

	//! Init
	//! Initializes the engine to an active state
	//! 
	bool Engine::Init() {
		/* ----------------------------------------------------------------
		* Initialize components
		* ---------------------------------------------------------------- */
		world = std::make_shared<World::World>();
		camera = std::make_unique<Player::Camera>(startPos, startRot, fov);
		player = std::make_shared<Player::Player>(std::move(camera));
		inputMgr = std::make_unique<InputMgr::InputMgr>(player, world);
		renderer = std::make_unique<Renderer::Renderer>(windowTitle, screenWidth, screenHeight, player, world, inputMgr);

		/* ----------------------------------------------------------------
		* Add world objects
		* ---------------------------------------------------------------- */
		// TODO: TEMP; Move to configuration
		World::Object obj(MaterialMgr::MATERIAL_ID::TEST_MAT,
			Util::Transform(Util::Vector3<double>(0, 0, 5),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
		world->AddObject(std::move(obj));

		World::Object obj2(MaterialMgr::MATERIAL_ID::TEST_MAT_2,
			Util::Transform(Util::Vector3<double>(3, 3, 5),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
		world->AddObject(std::move(obj2));

		World::Object obj3(MaterialMgr::MATERIAL_ID::TEST_MAT_3,
			Util::Transform(Util::Vector3<double>(-3, 3, 5),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
		world->AddObject(std::move(obj3));

		World::Object obj4(MaterialMgr::MATERIAL_ID::TEST_MAT_3,
			Util::Transform(Util::Vector3<double>(0, 0, 8),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
		world->AddObject(std::move(obj4));

		/* ----------------------------------------------------------------
		* Initialize renderer
		* ---------------------------------------------------------------- */
		bool success = renderer->Init();
		if (!success) {
			Util::Log::Error("Engine: Failed to initialize renderer");
			return false;
		}

		/* ----------------------------------------------------------------
		* Initialize render pool
		* ---------------------------------------------------------------- */
		renderPool.Init();

		isActive = true;
		return true;
	}

	//! IsActive
	//! Returns whether the engine is in an initialized and active state
	//! 
	bool Engine::IsActive() const {
		return this->isActive;
	}

	//! ProduceFrame
	//! Produces and displays a frame in the display window
	//! 
	bool Engine::DisplayFrame() {
		if (!IsActive()) {
			Util::Log::Error("Engine: Attempted to produce a frame when in an inactive state");
			return false;
		}

		//! Check for renderer activity
		if (!renderer->IsActive()) {
			Util::Log::Info("Engine: Renderer has been shut down");
			this->isActive = false;
			return false;
		}

		//! Generate world frame
		//renderer->...
		renderer->ProduceWorldFrame(player);
		renderer->DisplayFrame();

		return true;
	}

}; // namespace Engine