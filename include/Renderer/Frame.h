//!
//! Frame.cpp
//! Defines a modular frame buffer for pixel rendering
//! 
#pragma once

#include <cstdint>
#include <tuple>
#include "Util.h"
#include "Camera.h"



namespace Renderer {

	class Frame {
	private:
		std::string name;
		int posX, posY;
		int width, height;
		uint32_t* pixels;

	public:
		Frame(std::string name, int posX, int posY, int width, int height);
		~Frame();

		std::string GetName() const;
		int GetPosX() const;
		int GetPosY() const;
		int GetWidth() const;
		int GetHeight() const;
		const uint32_t* GetBuffer() const;
		uint32_t* GetBuffer();

		void SetPixel(int x, int y, uint32_t color);
		void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		uint32_t GetPixel(int x, int y);

	};

	//! FrameContext
	//! Provides context needed to populate a rendering frame
	//! 
	struct FrameContext {
		std::shared_ptr<Frame> frame;
		std::shared_ptr<Player::Camera> camera;

		FrameContext(std::shared_ptr<Frame> frame, std::shared_ptr<Player::Camera> camera);
	};

}; // namespace Renderer
