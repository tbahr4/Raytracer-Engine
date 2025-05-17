//!
//! Util.h
//! General utility functions
//! 
#pragma once

#include <iostream>


namespace Util {

	//! Logging definitions
	#define LOG_LEVEL 4
	#define LOG_INFO(message) if (LOG_LEVEL >= 4) { std::cout << "[INFO] " << message << std::endl; };
	#define LOG_DEBUG(message) if (LOG_LEVEL >= 3) { std::cout << "[DEBUG] " << message << std::endl; };
	#define LOG_WARNING(message) if (LOG_LEVEL >= 2) { std::cout << "[WARN] " << message << std::endl; };
	#define LOG_ERROR(message) if (LOG_LEVEL >= 1) { std::cout << "[ERROR] " << message << std::endl; };

	//! Vectors
	//! Defines mathematical vectors
	//! 
	template <typename T>
	class Vector3 {
	public:
		T x, y, z;
		Vector3(T x, T y, T z);
	};

	//! Transform
	//! Defines position, rotation, scale transforms of an object
	//! 
	class Transform {
	public:
		Vector3<double> position;
		Vector3<double> rotation;
		Vector3<double> scale;

		Transform(Vector3<double>& position, Vector3<double>& rotation, Vector3<double>& scale)
		: position(position)
		, rotation(rotation)
		, scale(scale)
		{}
	};

}; // namespace Util