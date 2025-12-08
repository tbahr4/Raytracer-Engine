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
		std::shared_ptr<Camera> camera;

	public:
		//! Constructors
		Player(std::shared_ptr<Camera> camera);

		//! Accessors
		std::shared_ptr<const Camera> GetCamera() const;
		std::shared_ptr<Camera> GetCamera();
		double GetMovementSpeed() const;
	};

}; // namespace Player