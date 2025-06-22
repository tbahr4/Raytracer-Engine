//!
//! Thread.cpp
//! Defines an abstraction for a base thread class 
//! 
#include "Thread.h"



namespace Util {

		//! Constructor
		//! 
		Thread::Thread(std::string name)
			: thread()
			, name(name)
			, isStarted(false)
			, args(nullptr)
		{}

		//! Destructor
		//! 
		Thread::~Thread() {
			if (thread.joinable()) {
				thread.join();
			}
		}

		//! Start
		//! Begins execution of the thread
		//! 
		void Thread::Start(void* args) {
			if (isStarted) {
				LOG_WARNING(name + ": Thread was requested to start when already active");
				return;
			}

			this->args = args;
			isStarted = true;
			thread = std::thread(&Thread::Runner, this);
		}

		//! Sleep
		//! Pauses the thread execution for the given time
		//! 
		void Thread::Sleep(unsigned int millis) {
			std::this_thread::sleep_for(std::chrono::milliseconds(millis));
		}

		//! Join
		//! Blocks execution until the thread is returned
		//! 
		void Thread::Join() {
			if (thread.joinable()) {
				thread.join();
			}
		}

		//! GetName
		//! Returns the name of the thread instance
		//! 
		const std::string& Thread::GetName() const {
			return name;
		}

		//! Runner
		//! Thread standard runner function
		//! 
		void Thread::Runner() {
			if (!Init()) {
				LOG_ERROR(name + ": Thread failed to initialize");
				return;
			}

			int retVal = Run(args);
			LOG_INFO(name + ": Thread exited with code " + std::to_string(retVal));
		}

}; // namespace Util