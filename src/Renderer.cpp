//!
//! Renderer.cpp
//! Central component for rendering logic
//! 
#include <Renderer.h>



namespace Renderer {

	//! Constructor
	//! 
	Renderer::Renderer(const char* windowTitle, int windowWidth, int windowHeight) 
	: window("WindowFrame", windowWidth, windowHeight)
	, display(windowTitle, windowWidth, windowHeight)
	{}

	//! Init
	//! Initializes the renderer to an active state
	//! 
	bool Renderer::Init() {
		bool success = this->display.Init();

		if (!success) {
			LOG_ERROR("Renderer initialization failed");
			return false;
		}

		LOG_INFO("Renderer initialized successfully");
		this->isInitialized = true;
		return true;
	}

	//! IsActive
	//! Returns the activity state of the renderer. Establishes readiness for processing commands
	//! 
	bool Renderer::IsActive() const {
		return this->isInitialized && display.IsActive();
	}

	//! SetupFrameTest
	//! Debug test command
	//! 
	void Renderer::SetupFrameTest() {
		if (!display.IsActive()) return;

		for (int x=0;x<100;x++)
			for (int y=0;y<100;y++)
				this->window.SetPixel(x+280,y+200, 0xFF0000FF);

		display.PollEvents();
		display.RenderFrame(this->window);
		SDL_Delay(1 / 60);
	}

}; // namespace Renderer