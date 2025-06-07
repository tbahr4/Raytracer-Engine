//!
//! Player.cpp
//! Stores and mananges the state of a player
//! 
#include "Player.h"



namespace Player {

	Player::Player(Camera* camera)
		: camera(camera)
	{}

	//! Accessors
	Camera* Player::GetCamera() { return camera; }
	double Player::GetMovementSpeed() const { return movementSpeed; }

}; // namespace Player 