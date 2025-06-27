//!
//! Log.h
//! Console logging methods
//! 
#pragma once

#include <mutex>
#include <iostream>



namespace Util {

	class Log {
	private:
		enum class LOG_LEVEL {
			ERROR = 1,
			WARNING = 2,
			DEBUG = 3,
			INFO = 4
		};

		static std::mutex mutex;
		static constexpr LOG_LEVEL logLevel = LOG_LEVEL::INFO;

	public:
		Log() = delete;
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		static void Info(std::string message);
		static void Debug(std::string message);
		static void Warn(std::string message);
		static void Error(std::string message);
	};

}; // namespace Util