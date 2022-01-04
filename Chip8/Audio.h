#pragma once

#include <array>
#include <cstdint>
#include <SFML/Audio.hpp>

class Audio
{
private:
	std::array<int16_t, 44100> raw;
	sf::SoundBuffer buffer;
	sf::Sound sound;

public:
	Audio()
	{
		for (uint32_t i = 0; i < 44100; i++)
		{
			raw[i] = (uint16_t)(((i / (44100 / 256) / 2) % 2) == 1 ? 10000 : -10000);
		}

		buffer.loadFromSamples(raw.data(), 44100, 1, 44100);
		sound.setBuffer(buffer);
	}

	void Play()
	{
		sound.play();
	}

	void Stop()
	{
		sound.stop();
	}
};

