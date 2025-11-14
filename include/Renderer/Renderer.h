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



namespace Renderer {

	class Renderer {
	private:
		Frame window;
		DisplayDriver display;
		std::shared_ptr<World::World> world;
		std::shared_ptr<InputMgr::InputMgr> inputMgr;

		//! Internal variables
		bool isInitialized = false;

		//! Properties
		const int maxRayDepth;
		const int resDownScale; // Downscaling factor of resolution

	public:
		//! Constructors
		Renderer(const char* windowTitle, int windowWidth, int windowHeight, std::shared_ptr<Player::Player> player, std::shared_ptr<World::World> world, std::shared_ptr<InputMgr::InputMgr> inputMgr, int maxRayDepth, int resDownScale);

		//! Initialization
		bool Init();

		//! Accessors
		bool IsActive() const;	

		//! Interface functions
		void ProduceWorldFrame(std::shared_ptr<Player::Player> player);
		void DisplayFrame();
		std::vector<RayMgr::Ray> GenerateRays(const Player::Camera* camera, int frameWidth, int frameHeight);
		Util::Vector3<double> CalcTotalLight(const RayMgr::Ray& ray) const;
		Frame* GetRawFrame();

		//! Accessors
		int GetWindowWidth() const;
		int GetWindowHeight() const;

	private:
		//! Helper functions
		Util::Vector3<double> _CalcTotalLightHelper(const RayMgr::Ray& ray, int depth) const;
	};

}; // namespace Renderer