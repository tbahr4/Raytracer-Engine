
//#include <DisplayDriver.h>

#include <SDL3/SDL.h>

constexpr int SCREEN_WIDTH = 500;
constexpr int SCREEN_HEIGHT = 350;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;


int main() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		exit(1);
	}

	gWindow = SDL_CreateWindow("Window", SCREEN_WIDTH, SCREEN_HEIGHT, NULL);
	if (gWindow == NULL) {
		exit(1);
	}

	gScreenSurface = SDL_GetWindowSurface(gWindow);
	if (gScreenSurface == NULL) {
		exit(1);
	}



    return 0;
}