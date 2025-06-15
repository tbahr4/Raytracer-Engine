//!
//! World.h
//! Manages the world state
//! 
#pragma once

#include <vector>
#include "Object.h"

#include "Singleton.h"


namespace World {

	class World {
	private:
		std::vector<Object> objects;

	public:
		int GetObjectCount() const;
		Object* GetObject(int index);
		void AddObject(Object& obj);

	};

}; // namespace World