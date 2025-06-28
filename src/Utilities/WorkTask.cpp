//!
//! WorkTask.cpp
//! Defines a generalized work task to be consumed and handled by a worker thread
//! 
#include "WorkTask.h"



namespace Util {

	int WorkTask::nextUID = 0;

	WorkTask::WorkTask() {
		this->uid = nextUID++;
	}

	int WorkTask::GetUID() const {
		return uid;
	}

}; // namespace Util