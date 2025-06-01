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



namespace Renderer {

	class Renderer {
	private:
		std::queue<Frame> renderQueue;
		Frame window;
		DisplayDriver display;

		//! Internal variables
		bool isInitialized = false;

	public:
		//! Constructors
		Renderer(const char* windowTitle, int windowWidth, int windowHeight);

		//! Initialization
		bool Init();

		//! Accessors
		bool IsActive() const;	

		//! Interface functions
		void SetupFrameTest();
		void RenderFrameTest(Player::Player* player, int screenWidth, int screenHeight);
	};

}; // namespace Renderer