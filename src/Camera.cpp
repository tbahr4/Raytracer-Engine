//!
//! Camera.cpp
//! Stores and mananges the state of a camera
//! 
#include "Camera.h"



namespace Player {

	//! Constructor
	//! 
	Camera::Camera(Util::Vector3<double> position, Util::Vector3<double> rotation, double fov)
		: position(position)
		, rotation(rotation)
		, fov(fov)
	{}

}; // namespace Player