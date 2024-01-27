#include "Machine.h"

void Machine::Run()
{
	sf::RenderWindow window(sf::VideoMode(1280, 640), "Chip-8");
	window.setFramerateLimit(60);

	sf::Texture texture;
	texture.create(64, 32);
	sf::Sprite frameBuffer(texture);
	frameBuffer.scale(20.0f, 20.0f);

	sf::FileInputStream stream;
	stream.open(romName);
	stream.read(&ram->ram[0x200], stream.getSize());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
			}
		}

		for (size_t i = 0; i < 16; i++)
		{
			keysPressed[i] = sf::Keyboard::isKeyPressed(keys[i]);
		}

		for (size_t i = 0; i < 1000 / 60; i++)
		{
			cpu->Decode();
		}

		if (cpu->dt > 0)
		{
			cpu->dt--;
		}

		if (cpu->st > 0)
		{
			audio->Play();
			cpu->st--;
		}
		else
		{
			audio->Stop();
		}
	
		display->UpdatePixels();
		texture.update(display->pixels.data());
		window.draw(frameBuffer);
		window.display();
	}
}
