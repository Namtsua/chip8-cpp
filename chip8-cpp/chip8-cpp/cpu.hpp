#include "constants.hpp"

class CPU
{
	public:
		uint8_t memory[Constants::MEMORY_SIZE]; // Chip 8 has 4k of memory
		uint8_t gfx[Constants::SCREEN_WIDTH * Constants::SCREEN_HEIGHT]; // Chip 8 screen size is usually 64 * 32
		uint8_t key[16];
		void initialize();
		void cycle();
		const bool& getDrawFlag() const;
		void setDrawFlag(const bool& state);
		
	private:
		uint8_t registers[Constants::REGISTER_COUNT]; // 16 8-bit registers (last is flag)
		uint16_t I; // Index register
		uint16_t pc; // Program Counter
		uint16_t sp; // Stack Pointer
		uint16_t opcode;
		uint16_t stack[Constants::STACK_SIZE];
		uint8_t delay_timer;
		uint8_t sound_timer;
		bool draw_flag;
};