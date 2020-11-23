#pragma once

#include <array>
#include "Memory.h"

class Display
{
private:
	//const uint32_t ink = 0x008000ff;
	const uint32_t ink = 0xff008000;
	//const uint32_t paper = 0x000000ff;
	const uint32_t paper = 0xff000000;
	const int32_t screenWidth = 64;
	const int32_t screenHeight = 32;

	Memory *ram;
	std::array<uint8_t, 64 * 32> screen{};

public:
	Display()
	{}

	Display(Memory *ram)
		: ram(ram)
	{}

	std::array<uint8_t, 64 * 32 * 4> pixels{};

	uint8_t DrawSprite(uint32_t x, uint32_t y, uint32_t height, uint16_t dataAddr);
	void Clear();
	void UpdatePixels();
};

