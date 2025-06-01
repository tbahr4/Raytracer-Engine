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
		double Magnitude() const {
			return std::sqrt(x * x + y * y + z * z);
		}

		Vector3<T> Normalized() const {
			double magnitude = Magnitude();
			if (magnitude == 0) {
				return Vector3<T>(0, 0, 0);
			}

			return Vector3<T>(x / magnitude, y / magnitude, z / magnitude);
		}

		void Normalize() {
			double magnitude = Magnitude();
			if (magnitude == 0) {
				return;
			}

			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}

		T Dot(const Vector3<T>& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		Vector3<T> Cross(const Vector3<T>& other) const {
			return Vector3<T>(
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
		Vector3<T> operator*(T scalar) const {
			return Vector3<T>(x * scalar, y * scalar, z * scalar);
		}
		friend Vector3<T> operator*(T scalar, const Vector3<T>& vec) {
			return vec * scalar;
		}
		Vector3<T> operator+(const Vector3<T>& other) const {
			return Vector3<T>(x + other.x, y + other.y, z + other.z);
		}
		Vector3<T> operator-(const Vector3<T>& other) const {
			return Vector3<T>(x - other.x, y - other.y, z - other.z);
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