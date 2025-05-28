//!
//! main.cpp
//! Main entry point
//! 

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "Renderer.h"
#include "World.h"
#include "Object.h"


int main() {
	//! Initialize components
	Renderer::Renderer* renderer = new Renderer::Renderer("Window", 640, 480);
	World::World* world = new World::World();

	//! Add object
	World::Object obj(0, Util::Transform(Util::Vector3<double>(10, 0, 0),
						 Util::Vector3<double>(0, 0, 0),
						 Util::Vector3<double>(1, 1, 1)), World::ShapeType::CUBE);
	world->AddObject(obj);


	renderer->Init();


	while (renderer->IsActive()) {
		renderer->SetupFrameTest();
	}

	delete renderer;
}



