#pragma once
#include <string>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>


namespace Constants
{
	const int SCREEN_WIDTH = 64;
	const int SCREEN_HEIGHT = 32;
	const int MEMORY_SIZE = 4096;
	const int REGISTER_COUNT = 16;
	const int STACK_SIZE = 16;
	const int KEYPAD_COUNT = 16;
	const int PROGRAM_START = 0x200;
	const int FONT_COUNT = 80;
}