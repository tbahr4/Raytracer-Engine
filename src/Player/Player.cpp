//!
//! Player.cpp
//! Stores and mananges the state of a player
//! 
#include "Player.h"



namespace Player {

	Player::Player(std::unique_ptr<Camera> camera)
		: camera(std::move(camera))
	{}

	//! Accessors
	const Camera* Player::GetCamera() const { return camera.get(); }
	Camera* Player::GetCamera() { return camera.get(); }
	double Player::GetMovementSpeed() const { return movementSpeed; }

}; // namespace Player 