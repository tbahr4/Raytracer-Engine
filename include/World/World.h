//!
//! World.h
//! Manages the world state
//! 
#pragma once

#include <vector>
#include "Object.h"



namespace World {

	class World {
	private:
		std::vector<Object> objects;

	public:
		int GetObjectCount() const;
		Object* GetObject(int index);
		const Object* GetObject(int index) const;
		void AddObject(Object& obj);

	};

}; // namespace World