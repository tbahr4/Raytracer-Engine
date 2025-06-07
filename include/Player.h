//!
//! Player.h
//! Stores and mananges the state of a player
//! 
#pragma once

#include "Camera.h"



namespace Player {

	class Player {
	private:
		//! Properties
		const double movementSpeed = 1;

		//! Component references
		Camera* camera;

	public:
		//! Constructors
		Player(Camera* camera);

		//! Accessors
		Camera* GetCamera();
		double GetMovementSpeed() const;
	};

}; // namespace Player