//!
//! Object.cpp
//! An instantiated, renderable world object
//! 
#include "Object.h"



namespace World {

	//! Constructor
	//! 
	Object::Object(int materialID, Util::Transform& transform, ShapeType shape) 
		: materialID(materialID)
		, transform(transform)
		, shape(shape)
	{}

	//! Accessors/Mutators
	//! 
	const Util::Transform& Object::GetTransform() const { return transform; }
	const Util::Vector3<double>& Object::GetPosition() const { return transform.position; }
	const Util::Rotation& Object::GetRotation() const { return transform.rotation; }
	const Util::Vector3<double>& Object::GetScale() const { return transform.scale; }

}; // namespace World