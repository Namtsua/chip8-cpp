#pragma once
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <thread>

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