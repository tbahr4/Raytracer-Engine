//!
//! WorkTask.h
//! Defines a generalized work task to be consumed and handled by a worker thread
//! 
#pragma once



namespace Util {

	class WorkTask {
	protected:
		int uid;

	public:
		WorkTask();
		int GetUID() const;

	private:
		static int nextUID;
	};

}; // namespace Util