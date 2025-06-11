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

constexpr int screenWidth = 1280;
constexpr int screenHeight = 960;
constexpr double fov = 60;



int main() {
	//! Initialize components
	World::World* world = new World::World();
	Player::Camera* camera = new Player::Camera(Util::Vector3<double>(0, 0, 0), Util::Rotation(0, 0, 0), fov);
	Player::Player* player = new Player::Player(camera);
	InputMgr::InputMgr* inputMgr = new InputMgr::InputMgr(player, world);
	Renderer::Renderer* renderer = new Renderer::Renderer("Raytracer", screenWidth, screenHeight, player, world, inputMgr);

	//! Add objects
	World::Object obj(MaterialMgr::MATERIAL_ID::TEST_MAT,
					  Util::Transform(Util::Vector3<double>(0, 0, 5),
					  Util::Rotation(0, 0, 0),
					  Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
	world->AddObject(obj);

	World::Object obj2(MaterialMgr::MATERIAL_ID::TEST_MAT_2,
		Util::Transform(Util::Vector3<double>(3, 3, 5),
			Util::Rotation(0, 0, 0),
			Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
	world->AddObject(obj2);

	World::Object obj3(MaterialMgr::MATERIAL_ID::TEST_MAT_3,
		Util::Transform(Util::Vector3<double>(-3, 3, 5),
			Util::Rotation(0, 0, 0),
			Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
	world->AddObject(obj3);

	World::Object obj4(MaterialMgr::MATERIAL_ID::TEST_MAT_3,
		Util::Transform(Util::Vector3<double>(0, 0, 8),
			Util::Rotation(0, 0, 0),
			Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
	world->AddObject(obj4);


	//! Initialization
	renderer->Init();


	//! Engine loop
	while (renderer->IsActive()) {
		//renderer->SetupFrameTest();
		//renderer->RenderFrameTest(player, screenWidth, screenHeight);
		renderer->RenderFrame(player);

	}

	delete renderer;
}



