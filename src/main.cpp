#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include <DisplayDriver.h>

#include <Frame.h>



int main() {
	Renderer::DisplayDriver* display = new Renderer::DisplayDriver("Window", 640, 480);
	if (!display->Init()) {
		exit(1);
	}

	while (display->IsActive()) {
		display->PollEvents();
		display->SetupFrameTest();
		display->RenderFrame();
		SDL_Delay(1/60);
	}



	delete display;
}



