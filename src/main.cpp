//!
//! main.cpp
//! Main entry point
//! 

#include "Engine.h"


constexpr int nRenderThreads = 16;	// TODO: Parameterize/default to all(-1)



int main() {
	/* ----------------------------------------------------------------
	* Initialize engine
	* ---------------------------------------------------------------- */
	Engine::Engine engine = Engine::Engine(nRenderThreads);
	
	bool success = engine.Init();
	if (!success) {
		Util::Log::Error("main: Engine initialization failed");
		return 1;
	}

	/* ----------------------------------------------------------------
	* Main loop
	* ---------------------------------------------------------------- */
	using clock = std::chrono::high_resolution_clock;
	auto lastTime = clock::now();

	while (engine.IsActive()) {
		engine.DisplayFrame();

		auto currentTime = clock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;
		lastTime = currentTime;

		float deltaTime = elapsed.count();
		float fps = 1.0f / deltaTime;
		std::cout << "FPS: " << fps << std::endl;
	}

	return 0;
}
