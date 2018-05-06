#include "chip8.hpp"

void Chip8::initialize() 
{
	m_cpu.initialize();
}

bool Chip8::loadGame(const std::string& file) { return false; }

void Chip8::emulateCycle() 
{
	m_cpu.cycle();
}

bool Chip8::getDrawFlag() const { return m_cpu.getDrawFlag(); }

void Chip8::setKeys() {}
