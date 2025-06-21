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
		struct FRUVector {
			Util::Vector3<double> forward;
			Util::Vector3<double> right;
			Util::Vector3<double> up;
		};

	public:
		//! Constructors
		Camera(Util::Vector3<double> position, Util::Rotation rotation, double fov);

		//! Accessors/Mutators
		const Util::Vector3<double> GetPosition() const;
		Util::Vector3<double>& GetPosition();
		const Util::Rotation GetRotation() const;
		Util::Rotation& GetRotation();
		double GetFOV() const;

		//! Utility functions
		const Util::Vector3<double> GetForwardVector() const;
		const FRUVector GetFRUVector() const;

	};

}; // namespace Player