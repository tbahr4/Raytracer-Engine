//!
//! Renderer.h
//! Central component for rendering logic
//! 
#pragma once

#include <queue>
#include <vector>
#include "Frame.h"
#include "DisplayDriver.h"
#include "RayMgr.h"
#include "Player.h"
#include "Util.h"
#include "World.h"
#include "Skybox.h"
#include "ThreadPool.h"
#include "RenderThread.h"
namespace Engine {
	class Engine;
}



namespace Renderer {

	class Renderer {
	private:
		using RenderTaskList = std::vector<std::shared_ptr<Util::RenderTask>>;

	private:
		Frame window;
		std::vector<std::shared_ptr<FrameContext>> frames;
		DisplayDriver display;
		std::shared_ptr<World::World> world;
		std::shared_ptr<InputMgr::InputMgr> inputMgr;
		std::unique_ptr<World::Skybox> skybox;

		//! Internal variables
		bool isInitialized = false;

		std::unique_ptr<RenderTaskList> renderTasks;
		std::unique_ptr<Util::ThreadPool<Util::RenderThread>> renderPool;	// Rendering thread pool

		//! Properties
		const int maxRayDepth;
		const int resDownScale; // Downscaling factor of resolution

	public:
		//! Constructors
		Renderer(const char* windowTitle, int windowWidth, int windowHeight, std::shared_ptr<Player::Player> player, std::shared_ptr<World::World> world, std::shared_ptr<InputMgr::InputMgr> inputMgr, int maxRayDepth, int resDownScale);
		~Renderer();

		//! Initialization
		bool Init();

		//! Accessors
		bool IsActive() const;	

		//! Interface functions
		void AddFrame(std::string name, int x, int y, int sizeX, int sizeY, std::shared_ptr<Player::Camera> camera);
		void RenderFrames();
		void DisplayFrame();
		std::vector<RayMgr::Ray> GenerateRays(std::shared_ptr<Player::Camera> camera, int frameWidth, int frameHeight) const;
		Util::Vector3<double> CalcTotalLight(const RayMgr::Ray& ray) const;
		Frame* GetRawWindowFrame();

		//! Accessors
		int GetWindowWidth() const;
		int GetWindowHeight() const;

	private:
		void ProduceFrame(std::shared_ptr<FrameContext> frameCtx);

		//! Helper functions
		Util::Vector3<double> _CalcTotalLightHelper(const RayMgr::Ray& ray, int depth) const;
		Util::Vector3<double> GetSkyboxColor(const RayMgr::Ray& ray) const;

	};

}; // namespace Renderer