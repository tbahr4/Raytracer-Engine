//!
//! World.cpp
//! Manages the world state
//! 
#include "World.h"
#include "Util.h"



namespace World {

	//! GetObjectCount
	//! Returns the total number of objects in the world
	//! 
	int World::GetObjectCount() const {
		return this->objects.size();
	}

	//! GetObject
	//! Returns a reference to the object at the specified index
	//! 
	Object* World::GetObject(int index) {
		if (index >= GetObjectCount()) {
			Util::Log::Error("Attempted to retrieve world object using invalid index");
			return nullptr;
		}

		return &objects[index];
	}

	//! GetObject
	//! Returns a reference to the object at the specified index
	//! 
	const Object* World::GetObject(int index) const {
		if (index >= GetObjectCount()) {
			Util::Log::Error("Attempted to retrieve world object using invalid index");
			return nullptr;
		}

		return &objects[index];
	}

	//! AddObject
	//! Adds the specified object to the renderable world
	//! 
	void World::AddObject(Object& obj) {
		this->objects.push_back(obj);
	}

}; // namespace World