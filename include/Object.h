//!
//! Object.h
//! An instantiated, renderable world object
//! 
#pragma once

#include "Util.h"



namespace World {

	//! ShapeType
	//! Defines the shape of the object to render
	//! 
	enum ShapeType {
		CUBE,
		RECTANGLE,
		SPHERE,
	};

	class Object {
	private:
		int materialID;
		Util::Transform transform;
		ShapeType shape;

	public:
		//! Constructors
		Object(int materialID, Util::Transform& transform, ShapeType shape);
		Object(int materialID, Util::Vector3<double>& position, Util::Vector3<double>& rotation, Util::Vector3<double>& scale, ShapeType shape);

		//! Accessor/Mutator functions
		//MaterialType GetMaterial() const;
		const Util::Transform& GetTransform() const;
		const Util::Vector3<double>& GetPosition() const;
		const Util::Vector3<double>& GetRotation() const;
		const Util::Vector3<double>& GetScale() const;

	};

}; // namespace World