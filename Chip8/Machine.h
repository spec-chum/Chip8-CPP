#pragma once

#include "Cpu.h"
#include <string>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Machine
{
private:
	Cpu *cpu;
	Memory *ram;
	Display *display;
	Audio *audio;

	std::string romName;

	const std::array<sf::Keyboard::Key, 16> keys
    {
		sf::Keyboard::Key::X,
		sf::Keyboard::Key::Num1,
		sf::Keyboard::Key::Num2,
		sf::Keyboard::Key::Num3,
		sf::Keyboard::Key::Q,
		sf::Keyboard::Key::W,
		sf::Keyboard::Key::E,
		sf::Keyboard::Key::A,
		sf::Keyboard::Key::S,
		sf::Keyboard::Key::D,
		sf::Keyboard::Key::Z,
		sf::Keyboard::Key::C,
		sf::Keyboard::Key::Num4,
		sf::Keyboard::Key::R,
		sf::Keyboard::Key::F,
		sf::Keyboard::Key::V
	};

public:
	std::array<bool, 16> keysPressed{};

	Machine(std::string rom)
	{
		ram = new Memory;
		display = new Display(ram);
		audio = new Audio;
		cpu = new Cpu(display, ram, audio, keysPressed);

		romName = rom;
	}

	void Run();
};

