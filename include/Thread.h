#pragma once

#include <thread>

namespace test {

	class Thread {
	private:
		std::thread worker;

	public:
		//! Constructors
		Thread() {
			this->Init();
		};
		~Thread() {
			if (worker.joinable()) {
				worker.join();
			}
		}

		//! Disable thread copy
		Thread(const Thread&) final = delete;
		Thread& operator=(const Thread&) final = delete;

		//! Final functions
		void Start() final {
			worker = std::thread(&Thread::Run, this);
		}

		//! Virtual functions
		virtual void Init() {};
		virtual void Run() {};
		virtual void Stop() {};
	};

} // namespace test