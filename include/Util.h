//!
//! Util.h
//! 
//! General utility functions
//! 
#pragma once

#include <iostream>


namespace Util {

	#define LOG_LEVEL 4

	#define LOG_INFO(message) if (LOG_LEVEL >= 4) { std::cout << "[INFO] " << message << std::endl; };
	#define LOG_DEBUG(message) if (LOG_LEVEL >= 3) { std::cout << "[DEBUG] " << message << std::endl; };
	#define LOG_WARNING(message) if (LOG_LEVEL >= 2) { std::cout << "[WARN] " << message << std::endl; };
	#define LOG_ERROR(message) if (LOG_LEVEL >= 1) { std::cout << "[ERROR] " << message << std::endl; };

}; // namespace Util