//!
//! Log.cpp
//! Console logging methods
//! 
#include "Log.h"




namespace Util {

	std::mutex Log::mutex;

	//! Info
	//! Logs message with [INFO] tag
	//! 
	void Log::Info(std::string message) {
		if ((int)logLevel >= 4) {
			std::lock_guard<std::mutex> lock(mutex);
			std::cout << "[INFO] " << message << std::endl;
		}
	}

	//! Debug
	//! Logs message with [DEBUG] tag
	//! 
	void Log::Debug(std::string message) {
		if ((int)logLevel >= 3) {
			std::lock_guard<std::mutex> lock(mutex);
			std::cout << "[DEBUG] " << message << std::endl;
		}
	}

	//! Warn
	//! Logs message with [WARN] tag
	//! 
	void Log::Warn(std::string message) {
		if ((int)logLevel >= 2) {
			std::lock_guard<std::mutex> lock(mutex);
			std::cout << "[WARN] " << message << std::endl;
		}
	}

	//! Error
	//! Logs message with [ERROR] tag
	//! 
	void Log::Error(std::string message) {
		if ((int)logLevel >= 1) {
			std::lock_guard<std::mutex> lock(mutex);
			std::cout << "[ERROR] " << message << std::endl;
		}
	}

}; // namespace Util