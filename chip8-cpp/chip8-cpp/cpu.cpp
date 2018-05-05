#include "cpu.hpp"

void CPU::initialize()
{
	pc = 0x200;  // Program counter starts at 0x200
	opcode = 0;      // Reset current opcode	
	I = 0;      // Reset index register
	sp = 0;      // Reset stack pointer
}

void CPU::cycle()
{
	opcode = memory[pc] << 8 | memory[pc + 1]; // Fetch opcode

	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x000F)
			{
			case 0x0000: // 0x00E0: Clears the screen        
						 // Execute opcode
				break;

			case 0x000E: // 0x00EE: Returns from subroutine          
						 // Execute opcode
				break;

			default:
				printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			}
			break;
		case 0x0004:
			if (registers[(opcode & 0x00F0) >> 4] > (0xFF - registers[(opcode & 0x0F00) >> 8]))
				registers[0xF] = 1; // carry flag
			else
				registers[0xF] = 0; // clear carry flag
			registers[(opcode & 0x00F00) >> 8] += registers[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x2000:
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FFF;
			break;
		case 0x0033:
			memory[I] = registers[(opcode & 0x0F00) >> 8] / 100;
			memory[I + 1] = (registers[(opcode & 0x0F00) >> 8] / 10) & 10;
			memory[I + 2] = (registers[(opcode & 0x0F00) >> 8] % 100) % 10;
			pc += 2;
			break;
		case 0xD000:
		{
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x000F;
			unsigned short pixel;

			V[0xF] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = memory[I + yline];
				for (int xline = 0; xline < 8; xline++)
				{
					if ((pixel & (0x80 >> xline)) != 0)
					{
						if (gfx[(x + xline + ((y + yline) * 64))] == 1)
							V[0xF] = 1;
						gfx[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}

			drawFlag = true;
			pc += 2;
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

