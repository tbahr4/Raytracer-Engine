//!
//! Vector2.h
//! Mathematical vector of size 2
//! 
#pragma once



namespace Util {

	template <typename T>
	class Vector2 {
	public:
		T x, y;
		Vector2(T x, T y) : x(x), y(y) {}
		Vector2() : x(0), y(0) {}

		//! Utility functions
		double Magnitude() const {
			return std::sqrt(x * x + y * y);
		}

		Vector2<T> Normalized() const {
			double magnitude = Magnitude();
			if (magnitude == 0) {
				return Vector2<T>(0, 0);
			}

			return Vector2<T>(x / magnitude, y / magnitude);
		}

		void Normalize() {
			double magnitude = Magnitude();
			if (magnitude == 0) {
				return;
			}

			x /= magnitude;
			y /= magnitude;
		}

		void Reverse() {
			x *= -1;
			y *= -1;
		}

		Vector2<T> Reversed() const {
			return Vector2<T>(-x, -y);
		}

		T Dot(const Vector2<T>& other) const {
			return x * other.x + y * other.y;
		}

		Vector2<T> Cross(const Vector2<T>& other) const {
			return Vector2<T>(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
			);
		}

		double AngleBetween(const Vector2<T>& other) const {
			return std::acos(this->Dot(other) / (this->Magnitude() * other.Magnitude()));
		}

		//! Operators
		Vector2<T> operator*(T scalar) const {
			return Vector2<T>(x * scalar, y * scalar);
		}
		friend Vector2<T> operator*(T scalar, const Vector2<T>& vec) {
			return vec * scalar;
		}
		Vector2<T> operator+(const Vector2<T>& other) const {
			return Vector2<T>(x + other.x, y + other.y);
		}
		Vector2<T>& operator+=(const Vector2<T>& other) {
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2<T> operator-(const Vector2<T>& other) const {
			return Vector2<T>(x - other.x, y - other.y);
		}
		Vector2<T>& operator-=(const Vector2<T>& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}
	};

}; // namespace Util