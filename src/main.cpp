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
const Util::Vector3<double> startPos = Util::Vector3<double>( 0,0,0 );
const Util::Rotation startRot = Util::Rotation(0, 0, 0);
constexpr char* windowTitle = "Raytracer";



int main() {
	/* ----------------------------------------------------------------
	* Initialize components
	* ---------------------------------------------------------------- */
	std::shared_ptr<World::World> world = std::make_shared<World::World>();
	std::unique_ptr<Player::Camera> camera = std::make_unique<Player::Camera>(startPos, startRot, fov);
	std::shared_ptr<Player::Player> player = std::make_shared<Player::Player>(std::move(camera));
	std::shared_ptr<InputMgr::InputMgr> inputMgr = std::make_unique<InputMgr::InputMgr>(player, world);
	std::unique_ptr<Renderer::Renderer> renderer = std::make_unique<Renderer::Renderer>(windowTitle, screenWidth, screenHeight, player, world, inputMgr);

	/* ----------------------------------------------------------------
	* Add world objects
	* ---------------------------------------------------------------- */
	World::Object obj(MaterialMgr::MATERIAL_ID::TEST_MAT,
					  Util::Transform(Util::Vector3<double>(0, 0, 5),
					  Util::Rotation(0, 0, 0),
					  Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
	world->AddObject(std::move(obj));

	World::Object obj2(MaterialMgr::MATERIAL_ID::TEST_MAT_2,
		Util::Transform(Util::Vector3<double>(3, 3, 5),
			Util::Rotation(0, 0, 0),
			Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
	world->AddObject(std::move(obj2));

	World::Object obj3(MaterialMgr::MATERIAL_ID::TEST_MAT_3,
		Util::Transform(Util::Vector3<double>(-3, 3, 5),
			Util::Rotation(0, 0, 0),
			Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
	world->AddObject(std::move(obj3));

	World::Object obj4(MaterialMgr::MATERIAL_ID::TEST_MAT_3,
		Util::Transform(Util::Vector3<double>(0, 0, 8),
			Util::Rotation(0, 0, 0),
			Util::Vector3<double>(1, 1, 1)), World::ShapeType::SPHERE);
	world->AddObject(std::move(obj4));


	/* ----------------------------------------------------------------
	* Initialize renderer
	* ---------------------------------------------------------------- */
	renderer->Init();

	/* ----------------------------------------------------------------
	* Main loop
	* ---------------------------------------------------------------- */
	while (renderer->IsActive()) {
		renderer->ProduceWorldFrame(player);
		renderer->DisplayFrame();
	}
}



