//!
//! Util.h
//! General utility functions
//! 
#pragma once

#include "Constants.h"
#include "Log.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Rotation.h"
#include "Transform.h"


//! Additional functions
//! 

namespace Util {

	double Clamp(double value, double a, double b);
	double Wrap(double value, double a, double b);

}; // namespace Util