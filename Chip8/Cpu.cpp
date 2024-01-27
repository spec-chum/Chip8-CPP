#include "Cpu.h"

void Cpu::Decode()
{
	const auto opcode = ram->ram[pc];
	const auto param = opcode & 0xf;
	const auto data = ram->ram[pc + 1];

	// Helpers for when using Vx and Vy to make them easier to read
	const auto x = (uint8_t)param;
	const auto y = (uint8_t)((data >> 4) & 0xf);

	pc += 2;

	switch (opcode >> 4)
	{
	case 0x0:
		switch (data)
		{
		case 0xe0:	// CLS
			display->Clear();
			break;

		case 0xee:	// RET
			if (stack.size() > 0)
			{
				pc = stack.top();
				stack.pop();
			}
			break;
		}
		break;

	case 0x1:     // JP nnn
		pc = (uint16_t)((param << 8) | data);
		break;

	case 0x2:     // CALL nnn
		if (stack.size() < 15)
		{
			stack.push(pc);
			pc = (uint16_t)((param << 8) | data);
		}
		break;

	case 0x3:     // SE Vx, byte
		if (v[x] == data)
		{
			pc += 2;        // skip next instruction if equal
		}
		break;

	case 0x4:     // SNE Vx, byte
		if (v[x] != data)
		{
			pc += 2;        // skip next instruction if not equal
		}
		break;

	case 0x5:     // SE Vx, Vy
		if (v[x] == v[y])
		{
			pc += 2;        // skip next instruction if equal
		}
		break;

	case 0x6:     // LD Vx, byte
		v[x] = data;
		break;

	case 0x7:     // ADD Vx, byte
		v[x] += data;
		break;

	case 0x8:     // Maths ops
		switch (data & 0xf)
		{
		case 0x0:     // LD Vx, Vy
			v[x] = v[y];
			break;

		case 0x1:     // OR Vx, Vy
			v[x] |= v[y];
			break;

		case 0x2:     // AND Vx, Vy
			v[x] &= v[y];
			break;

		case 0x3:     // XOR Vx, Vy
			v[x] ^= v[y];
			break;

		case 0x4:     // ADC Vx, Vy
			// If result is over 255, we overflowed, so set carry flag, otherwise clear it
			v[0xf] = (uint8_t)(v[x] + v[y] > 255 ? 1 : 0);
			v[x] += v[y];
			break;

		case 0x5:     // SBC Vx, Vy
			// If Vx is more or equal Vy, we can't overflow, so set carry flag, otherwise clear it
			v[0xf] = (uint8_t)(v[x] >= v[y] ? 1 : 0);
			v[x] -= v[y];
			break;

		case 0x6:    // SHR Vx
			v[0xf] = (uint8_t)(v[x] & 1);
			v[x] >>= 1;
			break;

		case 0x7:   // SUBN Vx, Vy
			// If Vy is more or equal Vx, we can't overflow, so set carry flag, otherwise clear it
			v[0xf] = (uint8_t)(v[y] >= v[x] ? 1 : 0);
			v[x] = (uint8_t)(v[y] - v[x]);
			break;

		case 0xe:    // SHL Vx
			v[0xf] = (uint8_t)((v[x] & 128) != 0 ? 1 : 0);
			v[x] <<= 1;
			break;
		}
		break;

	case 0x9:     // SNE Vx, Vy
		if (v[x] != v[y])
		{
			pc += 2;    // skip next instruction if not equal
		}
		break;

	case 0xa:    // LD I, nnn
		i = (uint16_t)((param << 8) | data);
		break;

	case 0xb:    // JP V0, nnn
		pc = (uint16_t)((param << 8) | (data + v[0]));
		break;

	case 0xc:    // RND Vx, byte
		v[x] = (uint8_t)((rand() % 255) & data);
		break;

	case 0xd:   // DRW Vx, Vy, bytes
		v[0xf] = display->DrawSprite(v[x], v[y], data & 0xf, i);
		break;

	case 0xe:   // Keyboard
		switch (data)
		{
		case 0x9e:  // SKP Vx
			// Skip next instruction if key pressed
			if (keysPressed[v[x]])
			{
				pc += 2;
			}
			break;

		case 0xa1:  // SKNP Vx
			// Skip next instruction if key not pressed
			if (!keysPressed[v[x]])
			{
				pc += 2;
			}
			break;
		}
		break;

	case 0xf:   // Misc and keyboard
		switch (data)
		{
		case 0x7:   // LD Vx, DT
			v[x] = dt;
			break;

		case 0xa:   // LD Vx, K
			if (halt)
			{
				for (int k = 0; k < 16; k++)
				{
					if (keysPressed[k])
					{
						halt = false;
						v[x] = (uint8_t)k;
						return;
					}
				}
			}

			halt = true;
			pc -= 2;
			break;

		case 0x15:  // LD DT, Vx
			dt = v[x];
			break;

		case 0x18:  // LD ST, Vx
			st = v[x];
			break;

		case 0x1e:  // ADD I, Vx
			i += v[x];
			break;

		case 0x29:  // LD F, Vx
			i = (uint16_t)(v[x] * 5);
			break;

		case 0x33:  // LD B, Vx
			ram->ram[i + 0] = (uint8_t)(v[x] / 100);
			ram->ram[i + 1] = (uint8_t)((v[x] / 10) % 10);
			ram->ram[i + 2] = (uint8_t)(v[x] % 10);
			break;

		case 0x55:  // LD [I], Vx
			for (size_t reg = 0; reg <= x; reg++)
			{
				ram->ram[i + reg] = v[reg];
			}
			break;

		case 0x65:  // LD Vx, [I]
			for (size_t reg = 0; reg <= x; reg++)
			{
				v[reg] = ram->ram[i + reg];
			}
			break;
		}
		break;
	}
}
