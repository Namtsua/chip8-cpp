#pragma once
#include "constants.hpp"
#include "cpu.hpp"

using namespace std;

class Chip8
{
	public:
		void initialize();
		bool loadGame(const std::string& file);
		void emulateCycle();
		bool getDrawFlag() const;
		void setKeys();
	private:
		CPU m_cpu;
};
