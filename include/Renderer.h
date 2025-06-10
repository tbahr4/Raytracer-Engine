//!
//! Renderer.h
//! Central component for rendering logic
//! 
#pragma once

#include <queue>
#include "Frame.h"
#include "DisplayDriver.h"
#include "RayMgr.h"
#include "Player.h"
#include "Util.h"
#include "World.h"



namespace Renderer {

	class Renderer {
	private:
		std::queue<Frame> renderQueue;
		Frame window;
		DisplayDriver display;
		World::World* world;
		InputMgr::InputMgr* inputMgr;

		//! Internal variables
		bool isInitialized = false;

	public:
		//! Constructors
		Renderer(const char* windowTitle, int windowWidth, int windowHeight, Player::Player* player, World::World* world, InputMgr::InputMgr* inputMgr);

		//! Initialization
		bool Init();

		//! Accessors
		bool IsActive() const;	

		//! Interface functions
		void SetupFrameTest();
		void RenderFrameTest(Player::Player* player, int screenWidth, int screenHeight);
		void RenderFrame(Player::Player* player);

	private:
		std::vector<RayMgr::Ray> GenerateRays(Player::Camera* camera, int frameWidth, int frameHeight);
	};

}; // namespace Renderer