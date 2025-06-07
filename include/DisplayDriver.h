//!
//! DisplayDriver.h
//! Establishes an abstracted interface to the SDL3 display
//! 
#pragma once

#include <SDL3/SDL.h>
#include "Util.h"
#include "Frame.h"
#include "InputMgr.h"



namespace Renderer {

	class DisplayDriver {
	private:
		//! Properties
		const char* windowTitle;
		int windowWidth;
		int windowHeight;

		//! SDL Objects
		SDL_Window* window{};
		SDL_Renderer* renderer{};
		SDL_Texture* texture{};
		SDL_Event event;

		//! Internal variables
		bool isInitialized = false;
		void* pixels;	// Raw pixel buffer
		int pitch;		// bytes per row of the pixel buffer
		InputMgr::InputMgr* inputMgr;

	public:
		//! Constructors
		DisplayDriver(const char* windowTitle, int windowWidth, int windowHeight, Player::Player* player, World::World* world);
		~DisplayDriver();

		//! Initialization
		bool Init();

		//! Accessors
		bool IsActive() const;

		//! Interface functions
		void PollEvents();
		void RenderFrame(const Frame& frame);

	};

}; // namespace Renderer
