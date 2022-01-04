#pragma once

#include <array>
#include <memory>
#include <stack>
#include "Memory.h"
#include "Display.h"
#include "Audio.h"

class Cpu
{
private:
	Memory *const ram;
	Display *const display;
	Audio *const audio;
	std::stack<uint16_t> stack{};
	std::array<bool, 16> &keysPressed;

	// Registers
	std::array<uint8_t, 16> v{};
	uint16_t i;
	uint16_t pc;

	bool halt;

public:
	uint8_t dt;
	uint8_t st;
	void Decode();

	Cpu(Display *display, Memory *ram, Audio *audio, std::array<bool, 16> &keysPressed)
		: display(display), ram(ram), audio(audio), keysPressed(keysPressed), pc(0x200), dt(0), st(0)
	{}
};

