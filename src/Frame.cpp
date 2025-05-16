//!
//! Frame.cpp
//! Defines a modular frame buffer for pixel rendering
//! 
#include <Frame.h>


namespace Renderer {

	//! Constructor
	//! Creates a frame buffer of specified size
	//! 
	Frame::Frame(std::string name, int width, int height) {
		this->name = name;
		this->width = width;
		this->height = height;
		this->pixels = new uint32_t[(double)width * (double)height]();
	}

	//! Destructor
	//! 
	Frame::~Frame() {
		delete this->pixels;
	}

	//! GetName
	//! Returns the name of the frame
	//! 
	std::string Frame::GetName() const {
		return name;
	}

	//! GetWidth
	//! Returns the number of pixels horizontally
	//!
	int Frame::GetWidth() const {
		return this->width;
	}

	//! GetHeight
	//! 
	int Frame::GetHeight() const {
		return this->height;
	}

	//! SetPixel
	//! Sets a pixel at the given position to the provided color
	void Frame::SetPixel(int x, int y, uint32_t color) {
		if (x >= width || y >= height) {
			LOG_WARNING("Attempted to set pixel outside the boundaries of frame");
			return;
		}

		this->pixels[x + (width * y)] = color;
	}

	//! SetPixel
	//! Sets a pixel at the given position to the provided color
	void Frame::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		this->SetPixel(x, y, (r << 6 | g << 4 | b << 2 | a));
	}

	uint32_t Frame::GetPixel(int x, int y) {
		if (x >= width || y >= height) {
			LOG_WARNING("Attempted to get pixel outside the boundaries of frame");
			return 0;
		}

		return pixels[x + (width * y)];
	}

}; // namespace Renderer
