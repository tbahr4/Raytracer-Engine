//!
//! Renderer.h
//! Central component for rendering logic
//! 
#pragma once

#include <queue>
#include <vector>
#include "Frame.h"
#include "DisplayDriver.h"
#include "RayMgr.h"
#include "Player.h"
#include "Util.h"
#include "World.h"



namespace Renderer {

	class Renderer {
	private:
		//std::queue<std::pair<Util::Vector2<double>, Frame*>> renderQueue; // TODO: This should be a processing queue for calling functions, not creating a frame to apply
		Frame window;
		DisplayDriver display;
		World::World* world;
		InputMgr::InputMgr* inputMgr;

		//! Internal variables
		bool isInitialized = false;

		//! Properties
		const int maxRayDepth = 1;	// TODO: Configurable

	public:
		//! Constructors
		Renderer(const char* windowTitle, int windowWidth, int windowHeight, Player::Player* player, World::World* world, InputMgr::InputMgr* inputMgr);

		//! Initialization
		bool Init();

		//! Accessors
		bool IsActive() const;	

		//! Interface functions
		void ProduceWorldFrame(Player::Player* player);
		void DisplayFrame();

	private:
		std::vector<RayMgr::Ray> GenerateRays(Player::Camera* camera, int frameWidth, int frameHeight);
		Util::Vector3<double> CalcTotalLight(const RayMgr::Ray& ray, int maxDepth);

		std::vector<RayMgr::Ray> GetDiffuseRays(RayMgr::CollisionInfo* colInfo);
		RayMgr::Ray GetReflectionRay(const RayMgr::Ray& ray, RayMgr::CollisionInfo* colInfo);
		RayMgr::Ray GetRefractionRay(const RayMgr::Ray& ray, RayMgr::CollisionInfo* colInfo);

		//! Helper functions
		Util::Vector3<double> _CalcTotalLightHelper(const RayMgr::Ray& ray, int depth, int maxDepth);
	};

}; // namespace Renderer