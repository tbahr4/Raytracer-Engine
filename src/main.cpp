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
	while (engine.IsActive()) {
		engine.DisplayFrame();
	}

	return 0;
}
