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

//#define SINGLE_THREADED



namespace Engine {

	namespace Config {
		//! General
		constexpr char* WINDOW_TITLE = "Raytracer";
		constexpr int NUM_RENDER_THREADS = 8;
		constexpr int SCREEN_WIDTH = 1920*2;
		constexpr int SCREEN_HEIGHT = 1080*2;
		constexpr double FOV = 60;
		const Util::Vector3<double> START_POS = { 0, 0, 0 };
		const Util::Rotation START_ROT = { 0, 0, 0 };
		constexpr double MOUSE_SENSITIVITY = .0025;
		constexpr double ROTATION_SPEED = 0.1;
		constexpr int MAX_RAY_DEPTH = 15; // Maximum number of ray bounces to render
		constexpr int NUM_RAYS_PER_TASK = 1000; // Number of ray traces to be packaged per worker thread
		constexpr int RESOLUTION_DOWN_SCALE = 4; // Scales down the ray count (e.g., NxN pixels per ray)

		//! Rendering

		//! Lighting
		const Util::Vector3<double> FLOOR_COLOR = { 45,45,45 };
		const Util::Vector3<double> CEILING_COLOR = { 75, 75, 255 };
		const Util::Vector3<double> LIGHT_COLOR = { 255,255,255 };
	}

	class Engine {
	private:
		bool isActive;				// Stores whether engine is initialized and active
		Util::ThreadPool<Util::RenderThread> renderPool;	// Rendering thread pool

		//! Sub-components
		std::shared_ptr<World::World> world;
		std::unique_ptr<Player::Camera> camera;
		std::shared_ptr<Player::Player> player;
		std::shared_ptr<InputMgr::InputMgr> inputMgr;
		std::unique_ptr<Renderer::Renderer> renderer;

		std::vector<std::shared_ptr<Util::RenderTask>> tasks;

	public:
		Engine();
		~Engine();
		
		//! Interface functions
		bool Init();
		bool IsActive() const;
		bool DisplayFrame();
	};

}; // namespace Engine