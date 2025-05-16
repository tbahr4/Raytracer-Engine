//!
//! Frame.cpp
//! Defines a modular frame buffer for pixel rendering
//! 
#pragma once

#include <cstdint>
#include <tuple>
#include <Util.h>



namespace Renderer {

	class Frame {
	private:
		std::string name;
		int width;
		int height;
		uint32_t* pixels;

	public:
		Frame(std::string name, int width, int height);
		~Frame();

		std::string GetName() const;
		int GetWidth() const;
		int GetHeight() const;

		void SetPixel(int x, int y, uint32_t color);
		void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		uint32_t GetPixel(int x, int y);

	};

}; // namespace Renderer
