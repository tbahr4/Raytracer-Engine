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

constexpr int screenWidth = 640;
constexpr int screenHeight = 480;
constexpr double fov = 90;


int main() {
	//! Initialize components
	Renderer::Renderer* renderer = new Renderer::Renderer("Window", screenWidth, screenHeight);
	World::World* world = new World::World();
	Player::Camera* camera = new Player::Camera(Util::Vector3<double>(0, 0, 0), Util::Rotation(0, 0, 0), fov);
	Player::Player* player = new Player::Player(camera);

	//! Add object
	World::Object obj(0, Util::Transform(Util::Vector3<double>(10, 0, 0),
						 Util::Rotation(0, 0, 0),
						 Util::Vector3<double>(1, 1, 1)), World::ShapeType::CUBE);
	world->AddObject(obj);


	//! Initialization
	renderer->Init();


	//! Engine loop
	while (renderer->IsActive()) {
		//renderer->SetupFrameTest();
		renderer->RenderFrameTest(player, screenWidth, screenHeight);
	}

	delete renderer;
}



