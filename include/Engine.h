//!
//! Engine.h
//! Manages the raytracing engine
//! 
#pragma once

#include <stdio.h>
#include "Renderer.h"
#include "World.h"
#include "Object.h"
#include "ThreadPool.h"
#include "RenderThread.h"



// TODO: Add to configuration
constexpr int screenWidth = 1280;
constexpr int screenHeight = 960;
constexpr double fov = 60;
const Util::Vector3<double> startPos = Util::Vector3<double>(0, 0, 0);
const Util::Rotation startRot = Util::Rotation(0, 0, 0);
constexpr char* windowTitle = "Raytracer";



namespace Engine {

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

	public:
		Engine(int nRenderThreads);
		~Engine();
		
		//! Interface functions
		bool Init();
		bool IsActive() const;
		bool DisplayFrame();
	};

}; // namespace Engine