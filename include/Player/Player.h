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
		const double movementSpeed = 0.1;

		//! Component references
		std::unique_ptr<Camera> camera;

	public:
		//! Constructors
		Player(std::unique_ptr<Camera> camera);

		//! Accessors
		const Camera* GetCamera() const;
		Camera* GetCamera();
		double GetMovementSpeed() const;
	};

}; // namespace Player