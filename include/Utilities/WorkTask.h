//!
//! WorkTask.h
//! Defines a generalized work task to be consumed and handled by a worker thread
//! 
#pragma once

#include <atomic>



namespace Util {

	class WorkTask {
	protected:
		int uid;

	public:
		WorkTask();
		int GetUID() const;
		void GetNewUID();

	private:
		static std::atomic<int> nextUID;
	};

}; // namespace Util