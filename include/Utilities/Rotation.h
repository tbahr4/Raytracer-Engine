//!
//! Util.h
//! Defines 3D rotation of yaw, pitch, and roll angles
//! 
#pragma once

#include <iostream>



namespace Util {

	class Rotation {
	public:
		double yaw, pitch, roll;
		Rotation(double yaw, double pitch, double roll) : yaw(yaw), pitch(pitch), roll(roll) {}
	};

}; // namespace Util