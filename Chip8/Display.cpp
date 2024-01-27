#include <cstdint>
#include "Display.h"

uint8_t Display::DrawSprite(const uint32_t x, const uint32_t y, const uint32_t height, const uint16_t dataAddr)
{
	uint8_t collision = 0;

	for (uint32_t dy = 0; dy < height; dy++)
	{
		auto bits = ram->ram[dataAddr + dy];

		for (uint32_t dx = 0; dx < 8; dx++)
		{
			const auto pos = ((y + dy) * 64) + (x + dx);
			if ((bits & 128) != 0)
			{
				if (screen[pos] == 1 && collision == 0)
				{
					collision = 1;
				}

				screen[pos] ^= 1;
			}

			bits <<= 1;
		}
	}

	return collision;
}

void Display::Clear()
{
	screen.fill(0);
}

void Display::UpdatePixels() const
{
	auto pixel = (uint32_t*)&pixels;

	for (uint32_t i = 0; i < 32 * 64; i++)
	{
		pixel[i] = screen[i] ? ink : paper;
	}
}
