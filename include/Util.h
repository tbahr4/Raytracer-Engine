//!
//! Util.h
//! General utility functions
//! 
#pragma once

#include <iostream>



namespace Util {

	//! Physical Constants
	//! 
	constexpr double PI = 3.14159265358979323846;

	//! Logging definitions
	//! 
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
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

		//! Utility functions
		void Normalize() {
			double magnitude = std::sqrt(std::pow(x,2) + std::pow(y,2) + std::pow(z,2));
			x = x / magnitude;
			y = y / magnitude;
			z = z / magnitude;
		}

		T Dot(const Vector3& other) {
			return x * other.x + y * other.y + z * other.z;
		}

		Vector3<T> Cross(const Vector3& other) {
			return Vector3(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			);
		}

		//! Constant vectors
		static Vector3<T> Right() { return Vector3<T>(1, 0, 0); }
		static Vector3<T> Up() { return Vector3<T>(0, 1, 0); }
		static Vector3<T> Forward() { return Vector3<T>(0, 0, 1); }

		//! Operators
		Vector3 operator*(T scalar) const {
			return Vector3(x * scalar, y * scalar, z * scalar);
		}
		friend Vector3 operator*(double scalar, const Vector3& vec) {
			return vec * scalar;
		}
		Vector3 operator+(const Vector3& other) {
			return Vector3(x + other.x, y + other.y, z + other.z);
		}
		Vector3 operator-(const Vector3& other) {
			return Vector3(x - other.x, y - other.y, z - other.z);
		}
	};
	

	//! Rotation
	//! Defines 3D rotation of yaw, pitch, and roll angles
	//! 
	class Rotation {
	public:
		double yaw, pitch, roll;
		Rotation(double yaw, double pitch, double roll) : yaw(yaw), pitch(pitch), roll(roll) {}
	};

	//! Transform
	//! Defines position, rotation, scale transforms of an object
	//! 
	class Transform {
	public:
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