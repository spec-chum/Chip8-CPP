#pragma once

#include <array>
#include <memory>
#include "Memory.h"

class Display
{
private:
	const uint32_t ink = 0xff008000;
	const uint32_t paper = 0xff000000;
	const int32_t screenWidth = 64;
	const int32_t screenHeight = 32;

	Memory *const ram;
	std::array<uint8_t, 64 * 32> screen{};

public:
	Display(Memory *ram)
		: ram(ram)
	{}

	std::array<uint8_t, 64 * 32 * 4> pixels{};

	uint8_t DrawSprite(const uint32_t x, const uint32_t y, const uint32_t height, const uint16_t dataAddr);
	void Clear();
	void UpdatePixels() const;
};

