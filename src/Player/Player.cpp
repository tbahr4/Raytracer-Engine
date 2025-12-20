//!
//! Player.cpp
//! Stores and mananges the state of a player
//! 
#include "Player.h"



namespace Player {

	Player::Player(std::shared_ptr<Camera> camera)
		: camera(camera)
	{}

	//! Accessors
	std::shared_ptr<const Camera> Player::GetCamera() const { return camera; }
	std::shared_ptr<Camera> Player::GetCamera() { return camera; }
	double Player::GetMovementSpeed() const { return movementSpeed; }

}; // namespace Player 