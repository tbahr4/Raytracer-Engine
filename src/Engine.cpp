//!
//! Engine.cpp
//! Manages the raytracing engine
//! 
#include "Engine.h"

// TODO: Make this configurable
//#define SINGLE_THREADED



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
		constexpr int nRaysPerTask = 1000;
		int nTasks = std::ceil(rays.size() / (double)nRaysPerTask);
		std::vector<Util::RenderTask> tasks(nTasks); // TODO: Do not create space every frame

		for (int taskI = 0; taskI < tasks.size(); taskI++) {
			tasks[taskI].startIdx = taskI * nRaysPerTask;
			tasks[taskI].endIdx = std::min(tasks[taskI].startIdx + nRaysPerTask, (int)rays.size());
			tasks[taskI].rays = &rays;
			tasks[taskI].renderer = renderer.get();
		}

		//! Add tasks to render pool
		renderPool.AddTasks(tasks);

		renderPool.WaitIdle();
#endif


		renderer->DisplayFrame();
		return true;
	}

}; // namespace Engine