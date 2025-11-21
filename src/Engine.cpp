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
		, renderPool("RenderPool", Config::NUM_RENDER_THREADS, Config::RESOLUTION_DOWN_SCALE)
		, tasks(std::ceil((Config::SCREEN_WIDTH * Config::SCREEN_HEIGHT / (Config::RESOLUTION_DOWN_SCALE * Config::RESOLUTION_DOWN_SCALE)) / (double)Config::NUM_RAYS_PER_TASK))
	{
		for (int i = 0; i < tasks.size(); i++) {
			tasks[i] = std::make_shared<Util::RenderTask>();
		}
	}

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
		camera = std::make_unique<Player::Camera>(Config::START_POS, Config::START_ROT, Config::FOV);
		player = std::make_shared<Player::Player>(std::move(camera));
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
				Util::Vector3<double>(10, 10, 10)),
			World::ShapeType::SPHERE);
		world->AddObject(std::move(obj5));

		/* ----------------------------------------------------------------
		* Initialize renderer
		* ---------------------------------------------------------------- */
		bool success = renderer->Init();
		if (!success) {
			Util::Log::Error("Engine: Failed to initialize renderer");
			return false;
		}

#ifndef SINGLE_THREADED
		/* ----------------------------------------------------------------
		* Initialize render pool
		* ---------------------------------------------------------------- */
		renderPool.Init();
#endif

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

		/* ----------------------------------------------------------------
		* Generate world frame
		* ---------------------------------------------------------------- */
#ifdef SINGLE_THREADED
		renderer->ProduceWorldFrame(player);
		
#else
		//! Get rays to trace
		std::vector<Renderer::RayMgr::Ray> rays = renderer->GenerateRays(player.get()->GetCamera(), renderer->GetWindowWidth(), renderer->GetWindowHeight());

		//! Split into rendering tasks
		int nTasks = std::ceil(rays.size() / (double)Config::NUM_RAYS_PER_TASK);
		assert(nTasks == tasks.size());

		for (int taskI = 0; taskI < tasks.size(); taskI++) {
			tasks[taskI]->GetNewUID();
			tasks[taskI]->startIdx = taskI * Config::NUM_RAYS_PER_TASK;
			tasks[taskI]->endIdx = std::min(tasks[taskI]->startIdx + Config::NUM_RAYS_PER_TASK, (int)rays.size());
			tasks[taskI]->rays = &rays;
			tasks[taskI]->renderer = renderer.get();
		}

		//! Add tasks to render pool
		Util::Log::Debug("Adding " + std::to_string(tasks.size()) + " tasks");
		renderPool.AddTasks(tasks);

		renderPool.WaitIdle();
#endif


		renderer->DisplayFrame();
		return true;
	}

}; // namespace Engine