//!
//! Player.h
//! Stores and mananges the state of a player
//! 
#pragma once

#include "Camera.h"



namespace Player {

	class Player {
	private:
		Camera* camera;

	public:
		//! Constructors
		Player(Camera* camera);

		//! Accessors
		Camera* GetCamera();
	};

}; // namespace Player