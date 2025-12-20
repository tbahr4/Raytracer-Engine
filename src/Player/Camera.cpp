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
	//! -Z: forward
	//! +X: right
	//! +Y: up
	//! 
	Util::Vector3<double> Camera::GetForwardVector() const {
		return Util::Vector3<double>(
			std::cos(rotation.pitch) * std::sin(rotation.yaw),
			std::sin(rotation.pitch),
			-std::cos(rotation.pitch) * std::cos(rotation.yaw)
		);
	}

	//! GetFRUVector
	//! Returns the forward, right, and up direction vectors of the camera
	//! 
	//! -Z: forward
	//! +X: right
	//! +Y: up
	//! 
	const Camera::FRUVector Camera::GetFRUVector() const {
		Camera::FRUVector fruVector;

		double roll = rotation.roll;
		double pitch = rotation.pitch;
		double yaw = rotation.yaw;

		fruVector.right = Util::Vector3<double>(cos(yaw)*cos(roll) + sin(yaw)*sin(pitch)*sin(roll), 
												cos(pitch)*sin(roll), 
												-sin(yaw)*cos(roll));

		fruVector.up = Util::Vector3<double>(-cos(yaw)*sin(roll) + sin(yaw)*sin(pitch)*cos(roll),
											 cos(pitch)*cos(roll), 
											 sin(yaw)*sin(roll) + cos(yaw)*sin(pitch)*cos(roll));

		fruVector.forward = Util::Vector3<double>(sin(yaw)*cos(pitch),
												  -sin(pitch),
												  cos(yaw)*cos(pitch)).Reversed();

		return fruVector;
	}

	//! Accessors/Mutators
	const Util::Vector3<double>& Camera::GetPosition() const { return position; }
	Util::Vector3<double>& Camera::GetPosition() { return position; }
	const Util::Rotation& Camera::GetRotation() const { return rotation; }
	Util::Rotation& Camera::GetRotation() { return rotation; }
	double Camera::GetFOV() const { return fov; }

}; // namespace Player