//!
//! Util.cpp
//! General utility functions
//! 
#include "Util.h"



//! Additional functions
//! 

namespace Util {

	double Clamp(double value, double minVal, double maxVal) {
		if (maxVal < minVal) {
			Log::Error("Clamp: Invalid [minVal,maxVal] interval");
			throw std::exception("Clamp: Invalid [minVal,maxVal] interval");
		}
		if (value <= minVal) {
			return minVal;
		}
		if (value >= maxVal) {
			return maxVal;
		}

		return value;
	}

	double Wrap(double value, double minVal, double maxVal) {
		if (maxVal < minVal) {
			Log::Error("Wrap: Invalid [minVal,maxVal] interval");
			throw std::exception("Wrap: Invalid [minVal,maxVal] interval");
		}

		double range = maxVal - minVal;
		if (range == 0) {
			return minVal;
		}

		double wrapped = std::fmod(value - minVal, range);
		if (wrapped < 0.0) wrapped += range;
		return wrapped + minVal;
	}

}; // namespace Util