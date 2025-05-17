#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include <Renderer.h>




int main() {
	Renderer::Renderer* renderer = new Renderer::Renderer("Window", 640, 480);
	renderer->Init();


	while (renderer->IsActive()) {
		renderer->SetupFrameTest();
	}

	delete renderer;
}



