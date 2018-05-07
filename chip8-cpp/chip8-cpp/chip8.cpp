#include "chip8.hpp"

void Chip8::initialize() 
{
	m_cpu.initialize();
}

bool Chip8::loadGame(const string& file) 
{
	ifstream in_file(file.c_str(), ios::in | ios::binary);
	if (!in_file.is_open())
	{
		fprintf(stderr, "Failed to open ROM file\n");
		return false;
	}

	in_file.seekg(0, in_file.end); // go to end of file
	int length = in_file.tellg(); // get file size
	in_file.seekg(0, in_file.beg); // go back to beginning of file
	in_file.read((char *)&m_cpu.memory[Constants::PROGRAM_START], length); // read program into memory
	in_file.close();

	return true;
}

void Chip8::emulateCycle() 
{
	m_cpu.cycle();
}

bool Chip8::getDrawFlag() const { return m_cpu.getDrawFlag(); }

void Chip8::setKeys() {}
