//!
//! DisplayDriver.cpp
//! Establishes an abstracted interface to the SDL3 display
//! 
#include "DisplayDriver.h"



namespace Renderer {

	//! Constructor
	//! 
	DisplayDriver::DisplayDriver(const char* windowTitle, int windowWidth, int windowHeight, Player::Player* player, World::World* world, InputMgr::InputMgr* inputMgr)
		: windowTitle(windowTitle)
		, windowWidth(windowWidth)
		, windowHeight(windowHeight)
		, inputMgr(inputMgr)
	{}

	//! Destructor
	//!
	DisplayDriver::~DisplayDriver() {
		if (texture) SDL_DestroyTexture(texture);
		if (renderer) SDL_DestroyRenderer(renderer);
		if (window) SDL_DestroyWindow(window);
		SDL_Quit();
		delete inputMgr;
	}

	//! Init
	//! Initializes the SDL layer
	//! 
	bool DisplayDriver::Init() {
		//! Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			LOG_ERROR("SDL_Init Error: " + (std::string)SDL_GetError())
				return false;
		}

		//! Initialize window
		this->window = SDL_CreateWindow(windowTitle, windowWidth, windowHeight, NULL);
		if (!window) {
			return false;
		}

		//! Initialize renderer
		this->renderer = SDL_CreateRenderer(window, nullptr);
		if (!renderer) {
			return false;
		}

		//! Initialize texture for pixel buffer
		this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, this->windowWidth, this->windowHeight);
		if (!texture) {
			LOG_ERROR("SDL_CreateTexture Error: " + (std::string)SDL_GetError());
			return false;
		}

		LOG_INFO("Display driver initialized successfully");
		this->isInitialized = true;
		return true;
	}

	//! IsActive
	//! Returns the activity state of the display driver. Establishes readiness for processing commands
	//! 
	bool DisplayDriver::IsActive() const {
		return this->isInitialized;
	}

	//! PollEvents
	//! Processes input events from the user
	//! 
	void DisplayDriver::PollEvents() {
		if (!IsActive()) return;

		while (SDL_PollEvent(&this->event)) {
			if (event.type == SDL_EVENT_QUIT ||
				(event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)) {
				this->isInitialized = false;
			}
			else {
				// Handle key events within InputManager
				inputMgr->HandleEvent(event);
			}
		}
	}

	//! RenderFrame
	//! Renders the provided frame buffer to the window
	//! 
	void DisplayDriver::RenderFrame(const Frame& frame) {
		if (!IsActive()) return;

		//! Set texture
		SDL_LockTexture(texture, NULL, &pixels, &pitch);
		memcpy(pixels, frame.GetBuffer(), pitch * this->windowHeight);
		SDL_UnlockTexture(texture);

		//! Render texture
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

};	// namespace Renderer
