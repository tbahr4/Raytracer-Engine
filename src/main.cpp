#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include <DisplayDriver.h>



int main() {
	DisplayDriver* display = new DisplayDriver("Window", 640, 480);
	bool success = display->Init();

	if (success && display->IsActive()) {
		LOG_DEBUG("Successful init");
	}
	else {
		LOG_DEBUG("Init failed");
	}

	while (display->IsActive()) {
		display->PollEvent();
		display->SetupFrameTest();
		display->RenderFrame();
		SDL_Delay(1/60);
	}



	delete display;
}



