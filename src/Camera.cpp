//!
//! Camera.cpp
//! Stores and mananges the state of a camera
//! 
#include "Camera.h"



namespace Player {

	//! Constructor
	//! 
	Camera::Camera(Util::Vector3<double> position, Util::Rotation rotation, double fov)
		: position(position)
		, rotation(rotation)
		, fov(fov)
	{}

	//! GetForwardVector
	//! Returns the forward direction vector of the camera
	//! 
	Util::Vector3<double> Camera::GetForwardVector() {
		Util::Vector3<double> forward = Util::Vector3<double>(
			std::cos(rotation.pitch) * std::sin(rotation.yaw),
			std::sin(rotation.pitch),
			std::cos(rotation.pitch) * std::cos(rotation.yaw)
		);

		return forward;
	}

}; // namespace Player