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

	//! GetFRUVector
	//! Returns the forward, right, and up direction vectors of the camera
	//! 
	Camera::FRUVector Camera::GetFRUVector() {
		// Forward vector
		Util::Vector3<double>& camForward = GetForwardVector();
		double roll = rotation.roll * Util::PI / 180;

		// World reference vector for RD vectors
		Util::Vector3<double> worldRefVec = Util::Vector3<double>::Up();
		if (std::abs(camForward.Dot(worldRefVec))) {
			worldRefVec = Util::Vector3<double>::Forward();
		}

		// Compute world axis orthogonal vectors
		Util::Vector3<double> camRightOrth = -1 * camForward.Cross(worldRefVec);
		Util::Vector3<double> camUpOrth = camRightOrth.Cross(camForward);

		// Roll camera RU vectors
		Util::Vector3<double> camRight = camRightOrth * std::cos(roll) +
			camUpOrth * std::sin(roll);

		Util::Vector3<double> camUp = camUpOrth * std::cos(roll) -
			camRightOrth * std::sin(roll);

		// Setup output structure
		Camera::FRUVector fruVector;
		fruVector.forward = camForward;
		fruVector.right = camRight;
		fruVector.up = camUp;

		return fruVector;
	}

	//! Accessors/Mutators
	Util::Vector3<double>& Camera::GetPosition() { return position; }
	Util::Rotation& Camera::GetRotation() { return rotation; }
	double Camera::GetFOV() const { return fov; }

}; // namespace Player