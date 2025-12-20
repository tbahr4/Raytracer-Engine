//!
//! Engine.cpp
//! Manages the raytracing engine
//! 
#include "Engine.h"





namespace Engine {

	//! Constructor
	//! 
	Engine::Engine()
		: isActive(false)
	{}

	//! Init
	//! Initializes the engine to an active state
	//! 
	bool Engine::Init() {
		/* ----------------------------------------------------------------
		* Initialize components
		* ---------------------------------------------------------------- */
		world = std::make_shared<World::World>();
		camera = std::make_shared<Player::Camera>(Config::START_POS, Config::START_ROT, Config::FOV);
		player = std::make_shared<Player::Player>(camera);
		inputMgr = std::make_unique<InputMgr::InputMgr>(player, world);
		renderer = std::make_unique<Renderer::Renderer>(Config::WINDOW_TITLE, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, player, world, inputMgr, Config::MAX_RAY_DEPTH, Config::RESOLUTION_DOWN_SCALE);

		/* ----------------------------------------------------------------
		* Add world objects
		* ---------------------------------------------------------------- */
		// TODO: TEMP; Move to configuration
		World::Object obj(MaterialMgr::MATERIAL_ID::TEST_MAT,
			Util::Transform(Util::Vector3<double>(0, 0, -5),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(1, 1, 1)), 
			World::ShapeType::SPHERE);
		world->AddObject(std::move(obj));

		World::Object obj2(MaterialMgr::MATERIAL_ID::TEST_MAT_2,
			Util::Transform(Util::Vector3<double>(3, 3, -5),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(1, 1, 1)), 
			World::ShapeType::SPHERE);
		world->AddObject(std::move(obj2));

		World::Object obj3(MaterialMgr::MATERIAL_ID::TEST_MAT_3,
			Util::Transform(Util::Vector3<double>(-3, 3, -5),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(1, 1, 1)), 
			World::ShapeType::SPHERE);
		world->AddObject(std::move(obj3));

		World::Object obj4(MaterialMgr::MATERIAL_ID::TEST_MAT_4,
			Util::Transform(Util::Vector3<double>(0, 0, -8),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(1, 1, 1)), 
			World::ShapeType::SPHERE);
		world->AddObject(std::move(obj4));

		World::Object obj5(MaterialMgr::MATERIAL_ID::TEST_MAT_4,
			Util::Transform(Util::Vector3<double>(0, -15, -5),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(3, 3, 3)),
			World::ShapeType::SPHERE);
		world->AddObject(std::move(obj5));

		World::Object obj6(MaterialMgr::MATERIAL_ID::TEST_MAT_5,
			Util::Transform(Util::Vector3<double>(0, 5, -5),
				Util::Rotation(0, 0, 0),
				Util::Vector3<double>(0.5, 0.5, 0.5)),
			World::ShapeType::SPHERE);
		world->AddObject(std::move(obj6));

		/* ----------------------------------------------------------------
		* Add rendering frames
		* ---------------------------------------------------------------- */
		renderer->AddFrame("Main Frame", 0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, camera);
		//renderer->AddFrame("Main Frame mini", Config::SCREEN_WIDTH -1920/2, 0, 1920/2, 1920/4, camera);


		/* ----------------------------------------------------------------
		* Initialize renderer
		* ---------------------------------------------------------------- */
		bool success = renderer->Init();
		if (!success) {
			Util::Log::Error("Engine: Failed to initialize renderer");
			return false;
		}

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
		renderer->RenderFrames();

		//! Display the world frame
		renderer->DisplayFrame();
		return true;
	}

}; // namespace Engine