//!
//! Skybox.h
//! Manages the world skybox
//! 
#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include "Vector3.h"
namespace fs = std::filesystem;



namespace World {

	const std::vector<std::string> SIDE_ORDER = { "front", "right", "back", "left", "up", "down" };

	class Skybox {
	public:
		enum Side {
			FRONT = 0,
			RIGHT = 1,
			BACK = 2,
			LEFT = 3,
			UP = 4,
			DOWN = 5
		};

		struct Image {
			unsigned char* pixels;
			int width;
			int height;
			int channels;
		};

	private:
		const std::string fileDir = "assets/images/skybox";
		const std::string imagePrefix = "skybox";
	
	private:
		std::vector<Image> sides;

	public:
		Skybox();
		~Skybox();

		Util::Vector3<double> GetPixel(double u, double v, Side side);

	private:
		const Image& GetImage(Side side);

	};

}; // namespace World