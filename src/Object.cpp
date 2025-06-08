//!
//! Object.cpp
//! An instantiated, renderable world object
//! 
#include "Object.h"



namespace World {

	//! Constructor
	//! 
	Object::Object(MaterialMgr::MATERIAL_ID materialID, Util::Transform& transform, ShapeType shape) 
		: materialID(materialID)
		, material(MaterialMgr::GetMaterial(materialID))
		, transform(transform)
		, shape(shape)
	{}

	//! Accessors/Mutators
	//! 
	const MaterialMgr::Material& Object::GetMaterial() const { return material; }
	const Util::Transform& Object::GetTransform() const { return transform; }
	const Util::Vector3<double>& Object::GetPosition() const { return transform.position; }
	const Util::Rotation& Object::GetRotation() const { return transform.rotation; }
	const Util::Vector3<double>& Object::GetScale() const { return transform.scale; }
	ShapeType Object::GetShapeType() const { return shape; }

}; // namespace World