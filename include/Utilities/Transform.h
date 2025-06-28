//!
//! Util.h
//! Defines position, rotation, scale transforms of an object
//! 
#pragma once

#include <iostream>



namespace Util {

	struct Transform {
		Vector3<double> position;
		Rotation rotation;
		Vector3<double> scale;

		Transform(Vector3<double>& position, Rotation& rotation, Vector3<double>& scale)
		: position(position)
		, rotation(rotation)
		, scale(scale)
		{}
	};

}; // namespace Util