//!
//! Camera.h
//! Stores and mananges the state of a camera
//! 
#pragma once

#include "Util.h"



namespace Player {

	class Camera {
	protected:
		Util::Vector3<double> position;
		Util::Vector3<double> rotation;
		double fov;

	public:
		Camera(Util::Vector3<double> position, Util::Vector3<double> rotation, double fov);
	};

}; // namespace Player