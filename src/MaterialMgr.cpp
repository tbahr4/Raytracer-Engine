//!
//! MaterialMgr.cpp
//! Stores and manages materials
//! 
#include "MaterialMgr.h"



namespace MaterialMgr {

	const Material& GetMaterial(MATERIAL_ID matID) {
		auto iter = materials.find(matID);
		if (iter == materials.end()) {
			//! Material does not exist
			return materials.at(MATERIAL_ID::AIR);
		}

		return iter->second;
	}

}; // namespace MaterialMgr