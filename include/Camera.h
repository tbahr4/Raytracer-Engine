//!
//! Camera.h
//! Stores and mananges the state of a camera
//! 
#pragma once

#include "Util.h"



namespace Player {

	class Camera {
	private:
		Util::Vector3<double> position;
		Util::Rotation rotation;
		double fov; // degrees

	public:
		//! Constructors
		Camera(Util::Vector3<double> position, Util::Rotation rotation, double fov);

		//! Accessors/Mutators
		Util::Vector3<double>& GetPosition();
		Util::Rotation& GetRotation();
		double GetFOV() const;

		//! Utility functions
		Util::Vector3<double> GetForwardVector();
	};

}; // namespace Player