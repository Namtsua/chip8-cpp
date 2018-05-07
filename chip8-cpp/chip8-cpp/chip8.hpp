#include "constants.hpp"
#include "cpu.hpp"

using namespace std;

class Chip8
{
	public:
		void initialize();
		bool loadGame(const std::string& file);
		void emulateCycle();
		CPU& getCPU();
		bool getDrawFlag() const;
		void setDrawFlag(const bool& state);
	private:
		CPU m_cpu;
};
