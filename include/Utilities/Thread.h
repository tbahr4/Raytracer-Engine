//!
//! Thread.h
//! Defines an abstraction for a base thread class 
//! 
#pragma once

#include <thread>
#include <string>
#include <chrono>
#include "Util.h"


namespace Util {

	class Thread {
	private:
		std::thread thread;
		std::string name;
		void* args;

		bool isStarted;

	public:
		Thread(std::string name);
		virtual ~Thread();
		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;

		//! Interface functions
		void Start(void* args);
		static void Sleep(unsigned int millis);
		void Join();
		const std::string& GetName() const;

	protected:
		//! Pure virtual functions
		virtual bool Init() = 0;
		virtual int Run(void* vArgs) = 0;

	private:
		void Runner();

	};

}; // namespace Util