#pragma once
#include "constants.hpp"

class CPU
{
	public:
		void initialize();
		void cycle();
		bool draw_flag;
		
	private:
		unsigned char registers[Constants::REGISTER_COUNT]; // 16 8-bit registers (last is flag)
		unsigned short I; // Index register
		unsigned short pc; // Program Counter
		unsigned short sp; // Stack Pointer
		unsigned short opcode;
		unsigned short stack[Constants::STACK_SIZE];
		unsigned char memory[Constants::MEMORY_SIZE]; // Chip 8 has 4k of memory
		unsigned char gfx[Constants::SCREEN_WIDTH * Constants::SCREEN_HEIGHT]; // Chip 8 screen size is usually 64 * 32
		unsigned char delay_timer;
		unsigned char sound_timer;
};