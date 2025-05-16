//!
//! DisplayDriver.h
//! 
//! Manages the display interface using SDL3
//! Singleton
//! 
#pragma once

#include <SDL3/SDL.h>
#include <Util.h>

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

public:
	DisplayDriver(const char* windowTitle, int windowWidth, int windowHeight) {
		this->windowTitle = windowTitle;
		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;
	}

	~DisplayDriver() {
		if (texture) SDL_DestroyTexture(texture);
		if (renderer) SDL_DestroyRenderer(renderer);
		if (window) SDL_DestroyWindow(window);
		SDL_Quit();
	}

	bool Init() {
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

		this->isInitialized = true;
		return true;
	}

	bool IsActive() const {
		return this->isInitialized;
	}

	void SetupFrameTest() {
		if (!IsActive()) return;

		SDL_LockTexture(texture, NULL, &pixels, &pitch);
		memset(pixels, 0, pitch * this->windowHeight);

		Uint32* pixels32 = (Uint32*)pixels;
		Uint32 white = 0xFF0000FF; // RGBA
		Uint32 white2 = 0x0000FFFF; // RGBA
		

		int square_size = 100;
		int start_x = (this->windowWidth - square_size) / 2;
		int start_y = (this->windowHeight - square_size) / 2;

		for (int y = 0; y < square_size; ++y) {
			for (int x = 0; x < square_size; ++x) {
				int px = start_x + x;
				int py = start_y + y;
				pixels32[py * (pitch / 4) + px] = white;
			}
		}

		SDL_UnlockTexture(texture);
	}

	void PollEvent() {
		if (!IsActive()) {
			LOG_WARNING("Attempting to poll inputs on an inactive display");
			return;
		}

		while (SDL_PollEvent(&this->event)) {
			if (event.type == SDL_EVENT_QUIT || 
				(event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)) {
				this->isInitialized = false;
			}
			else {
				// Handle events within InputManager
			}
		}
	}

	void RenderFrame() {
		if (!IsActive()) return;

		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}


};