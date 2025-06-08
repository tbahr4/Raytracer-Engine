//!
//! MaterialMgr.h
//! Manages available materials for object rendering
//! 
#pragma once

#include "Util.h"
#include <unordered_map>



namespace MaterialMgr {

	enum class MATERIAL_ID {
		AIR = 0,
		TEST_MAT = 1,
		TEST_MAT_2 = 2,
		TEST_MAT_3 = 3
	};

	struct Material {
		Util::Vector3<double> color;
		double reflectivity;
		double transparency;

		Material(Util::Vector3<double> color, double reflectivity, double transparency)
			: color(color)
			, reflectivity(reflectivity)
			, transparency(transparency)
		{}
	};

	const std::unordered_map<MATERIAL_ID, Material> materials = {
		{MATERIAL_ID::AIR,		  Material(Util::Vector3<double>(0,0,0), 0, 1)},
		{MATERIAL_ID::TEST_MAT,   Material(Util::Vector3<double>(0,0,255), 0, 0)},
		{MATERIAL_ID::TEST_MAT_2, Material(Util::Vector3<double>(255,50,200), 0, 0)},
		{MATERIAL_ID::TEST_MAT_3, Material(Util::Vector3<double>(50,255,50), 0, 0)}
	};

	const Material& GetMaterial(MATERIAL_ID matID);

}; // namespace MaterialMgr