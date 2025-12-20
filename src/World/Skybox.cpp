//!
//! Skybox.h
//! Manages the world skybox
//! 
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Skybox.h"



namespace World {

	Skybox::Skybox()
		: sides(SIDE_ORDER.size())
	{
		for (int sideI = 0; sideI < SIDE_ORDER.size(); sideI++) {
			fs::path filePath = fs::current_path() / fs::path(fileDir) / fs::path(imagePrefix + "_" + SIDE_ORDER[sideI] + ".png");
			int width, height, channels;
			unsigned char* pixels = stbi_load(filePath.string().c_str(), &width, &height, &channels, 3);

			Image img;
			img.pixels = pixels;
			img.width = width; 
			img.height = height;
			img.channels = channels;

			sides[sideI] = img;
		}
	}

	Skybox::~Skybox() {
		for (int sideI = 0; sideI < SIDE_ORDER.size(); sideI++) {
			stbi_image_free(sides[sideI].pixels);
		}
	}

	Util::Vector3<double> Skybox::GetPixel(double u, double v, Side side) {
		const Image& image = GetImage(side);

		if (u < 0 || u > 1 || v < 0 || v > 1) {
			throw std::invalid_argument("Skybox: Invalid pixel location");
		}

		//! Map UV to XY
		int x = u == 1 ? image.width : u * image.width;
		int y = v == 1 ? image.height : v * image.height;

		int pixelIdx = (x + (y * image.width)) * 3;

		unsigned char r = image.pixels[pixelIdx];
		unsigned char g = image.pixels[pixelIdx + 1];
		unsigned char b = image.pixels[pixelIdx + 2];

		return { 
			static_cast<double>(r),
			static_cast<double>(g),
			static_cast<double>(b)
		};
	}

	const Skybox::Image& Skybox::GetImage(Side side) {
		switch (side) {
		case Side::FRONT: return sides[0];
		case Side::RIGHT: return sides[1];
		case Side::BACK: return sides[2];
		case Side::LEFT: return sides[3];
		case Side::UP: return sides[4];
		case Side::DOWN: return sides[5];
		default:
			throw std::invalid_argument("Skybox: Invalid side");
		}
	}

}; // namespace World