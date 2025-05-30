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

		//! Accessors
		const Util::Vector3<double>& GetPosition() const { return position; }
		const Util::Rotation& GetRotation() const { return rotation; }
		double GetFOV() const { return fov; }

		//! Utility functions
		Util::Vector3<double> GetForwardVector();
	};

}; // namespace Player