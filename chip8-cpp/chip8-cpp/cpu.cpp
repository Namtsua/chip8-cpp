#include "cpu.hpp"

unsigned char fontset[Constants::FONT_COUNT] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void CPU::initialize()
{
	pc = Constants::PROGRAM_START;  // Program counter starts at 0x200
	opcode = 0;      // Reset current opcode	
	I = 0;      // Reset index register
	sp = 0;      // Reset stack pointer
	srand(time(NULL)); // Seed random number
	delay_timer = 0; // Reset delay timer
	sound_timer = 0; // Reset sound timer
	
	for (int i = 0; i < Constants::REGISTER_COUNT; i++) // clear registers
		registers[i] = 0;
	for (int i = 0; i < Constants::MEMORY_SIZE; i++) // clear memory
		memory[i] = 0;
	for (int i = 0; i < Constants::STACK_SIZE; i++) // clear stack
		stack[i] = 0;
	for (int i = 0; i < (Constants::SCREEN_WIDTH * Constants::SCREEN_HEIGHT); i++) // clear screen
		gfx[i] = 0;
	for (int i = 0; i < Constants::KEYPAD_COUNT; i++) // clear keys
		key[i] = 0;
	for (int i = 0; i < Constants::FONT_COUNT; i++) // store font in memory
		memory[i] = fontset[i];


}

void CPU::cycle()
{
	opcode = memory[pc] << 8 | memory[pc + 1]; // Fetch opcode

	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x000F)
			{
				case 0x0000:// 0x00E0: Clears the screen        
					for (int i = 0; i < (Constants::SCREEN_WIDTH * Constants::SCREEN_HEIGHT); i++)
						gfx[i] = 0;
					draw_flag = true;
					pc += 2;
					break;

				case 0x000E: // 0x00EE: Returns from subroutine
					--sp;
					pc = stack[sp];
					pc += 2;
					break;
				default:
					fprintf(stderr, "Unknown opcode: 0x%X\n", opcode);
					pc += 2;
					break;
			}
			break;		


		case 0x1000:
			pc = opcode & 0x0FFF;
			break;

		case 0x2000:
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FFF;
			break;

		case 0x3000:
			if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
			break;

		case 0x4000:
			if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
			break;

		case 0x5000:
			if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
			break;

		case 0x6000:
			registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			pc += 2;
			break;

		case 0x7000: //add vx, rr (immediate add)
			registers[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
			pc += 2;
			break;

		case 0x8000:
			switch (opcode & 0x000F)
			{
				case 0x0000: // Mov vx, vy
					registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0001:
					registers[(opcode & 0x0F00) >> 8] |= registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0002:
					registers[(opcode & 0x0F00) >> 8] &= registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0003:
					registers[(opcode & 0x0F00) >> 8] ^= registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0004: // Add vx, vy
					if (registers[(opcode & 0x00F0) >> 4] > (0xFF - registers[(opcode & 0x0F00) >> 8]))
						registers[0xF] = 1; // carry flag
					else
						registers[0xF] = 0; // clear carry flag
					registers[(opcode & 0x0F00) >> 8] += registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0005: // Sub vx, vy
					if (registers[(opcode & 0x0F00) >> 8] > (registers[(opcode & 0x00F0) >> 4]))
						registers[0xF] = 1;
					else
						registers[0xF] = 0;
					registers[(opcode & 0x0F00) >> 8] -= registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0006:
					registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0x1;
					registers[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;
					break;

				case 0x0007:
					if (registers[(opcode & 0x00F0) >> 4] > registers[(opcode & 0x0F00) >> 8])
						registers[0xF] = 1;
					else
						registers[0xf] = 0;
					registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				case 0x000E:
					registers[0xF] = registers[(opcode & 0x0F00) >> 8] >> 7;
					registers[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;
					break;

				default:
					fprintf(stderr, "Unknown opcode: 0x%X\n", opcode);
					pc += 2;
					break;
			}
		break;

		case 0x9000:
			if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
			break;

		case 0xA000:
			I = opcode & 0x0FFF;
			pc += 2;
			break;

		case 0xB000:
			pc = (opcode & 0x0FFF) + registers[0];
			break;

		case 0xC000:
			registers[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
			pc += 2;
			break;

		case 0xD000:
		{
			unsigned short x = registers[(opcode & 0x0F00) >> 8];
			unsigned short y = registers[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x000F;
			unsigned short pixel;

			registers[0xF] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = memory[I + yline];
				for (int xline = 0; xline < 8; xline++)
				{
					if ((pixel & (0x80 >> xline)) != 0)
					{
						if (gfx[(x + xline + ((y + yline) * 64))] == 1)
							registers[0xF] = 1;
						gfx[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}

			draw_flag = true;
			pc += 2;
		}
		break;

		case 0xE000:
			switch (opcode & 0x00FF)
			{
				// EX9E: Skips the next instruction 
				// if the key stored in VX is pressed
				case 0x009E:
					if (key[registers[(opcode & 0x0F00) >> 8]] != 0)
						pc += 4;
					else
						pc += 2;
					break;

				case 0x00A1:
					if (key[registers[(opcode & 0x0F00) >> 8]] == 0)
						pc += 4;
					else
						pc += 2;
					break;

				default:
					fprintf(stderr, "Unknown opcode: 0x%X\n", opcode);
					pc += 2;
					break;

			}
			break;

		case 0xF000:
			switch (opcode & 0x00FF)
			{
				case 0x0007:
					registers[(opcode & 0x0F00) >> 8] = delay_timer;
					pc += 2;
					break;

				case 0x000A:
				{ // create new scope for key_pressed
					bool key_pressed = false;
					for (int i = 0; i < Constants::KEYPAD_COUNT; i++)
					{
						if (key[i] != 0) // A key has been pressed
						{
							registers[(opcode & 0x0F00) >> 8] = i;
							key_pressed = true;
						}
					}
					if (!key_pressed) return;
					pc += 2;
					break;
				}

				case 0x0015:
					delay_timer = registers[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				case 0x0018:
					sound_timer = registers[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				case 0x001E:
					if (I + registers[(opcode & 0x0F00) >> 8] > 0xFFF) //check for overflow
						registers[0xF] = 1;
					else
						registers[0xF] = 0;
					I += registers[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				case 0x0029:
					I = registers[(opcode & 0x0F00) >> 8] * 0x5;
					pc += 2;
					break;

				case 0x0033:
					memory[I] = registers[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (registers[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (registers[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
					break;

				case 0x0055:
					for (int i = 0; i < ((opcode & 0x0F00) >> 8); i++) 
						memory[I + i] = registers[i];
					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
					break;

				case 0x0065:
					for (int i = 0; i < ((opcode & 0x0F00) >> 8); i++)
						registers[i] = memory[I + i];
					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
					break;

				default:
					fprintf(stderr, "Unknown opcode: 0x%X\n", opcode);
					pc += 2;
					break;
			}
			break;
		default:
			fprintf(stderr, "Unknown opcode: 0x%X\n", opcode);
	}

	// Update timers
	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0)
	{
		if (sound_timer == 1) fprintf(stderr, "BEEP!\n");
		--sound_timer;
	}

}

const bool& CPU::getDrawFlag() const
{
	return draw_flag;
}

void CPU::setDrawFlag(const bool& state)
{
	draw_flag = state;
}