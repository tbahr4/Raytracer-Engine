//!
//! main.cpp
//! Main entry point
//! 

#include "Engine.h"



int main() {
	/* ----------------------------------------------------------------
	* Initialize engine
	* ---------------------------------------------------------------- */
	Engine::Engine engine = Engine::Engine();
	
	bool success = engine.Init();
	if (!success) {
		Util::Log::Error("main: Engine initialization failed");
		return 1;
	}

	/* ----------------------------------------------------------------
	* Main loop
	* ---------------------------------------------------------------- */
	while (engine.IsActive()) {
		auto start = std::chrono::high_resolution_clock::now();
		engine.DisplayFrame();
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		Util::Log::Warn(std::to_string(1 / duration.count()) + " FPS");
	}

	return 0;
}
