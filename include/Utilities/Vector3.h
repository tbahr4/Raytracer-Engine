//!
//! Vector3.h
//! Mathematical vector of size 3
//! 
#pragma once



namespace Util {

	template <typename T>
	class Vector3 {
	public:
		T x, y, z;
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector3() : x(0), y(0), z(0) {}

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

		void Reverse() {
			x *= -1;
			y *= -1;
			z *= -1;
		}

		Vector3<T> Reversed() const {
			return Vector3<T>(-x, -y, -z);
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

		double AngleBetween(const Vector3<T>& other) const {
			return std::acos(this->Dot(other) / (this->Magnitude() * other.Magnitude()));
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
		Vector3<T>& operator+=(const Vector3<T>& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vector3<T> operator-(const Vector3<T>& other) const {
			return Vector3<T>(x - other.x, y - other.y, z - other.z);
		}
		Vector3<T>& operator-=(const Vector3<T>& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
	};

}; // namespace Util