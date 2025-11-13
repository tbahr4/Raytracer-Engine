//!
//! Engine.h
//! Manages the raytracing engine
//! 
#pragma once

#include <stdio.h>
#include <cassert>
#include "Renderer.h"
#include "World.h"
#include "Object.h"
#include "ThreadPool.h"
#include "RenderThread.h"



namespace Engine {

	namespace Config {
		constexpr char* WINDOW_TITLE = "Raytracer";
		constexpr int NUM_RENDER_THREADS = 15;
		constexpr int SCREEN_WIDTH = 1280;
		constexpr int SCREEN_HEIGHT = 960;
		constexpr double FOV = 60;
		const Util::Vector3<double> START_POS = Util::Vector3<double>(0, 0, 0);
		const Util::Rotation START_ROT = Util::Rotation(0, 0, 0);
		constexpr double MOUSE_SENSITIVITY = .0025;
		constexpr int MAX_RAY_DEPTH = 1; // Maximum number of ray bounces to render
		constexpr int NUM_RAYS_PER_TASK = 1000; // Number of ray traces to be packaged per worker thread
	}

	class Engine {
	private:
		bool isActive;				// Stores whether engine is initialized and active
		int nRenderThreads;			// Number of threads
		Util::ThreadPool<Util::RenderThread> renderPool;	// Rendering thread pool

		//! Sub-components
		std::shared_ptr<World::World> world;
		std::unique_ptr<Player::Camera> camera;
		std::shared_ptr<Player::Player> player;
		std::shared_ptr<InputMgr::InputMgr> inputMgr;
		std::unique_ptr<Renderer::Renderer> renderer;

		std::vector<Util::RenderTask> tasks;

	public:
		Engine();
		~Engine();
		
		//! Interface functions
		bool Init();
		bool IsActive() const;
		bool DisplayFrame();
	};

}; // namespace Engine