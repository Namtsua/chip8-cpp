#pragma once
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_events.h>
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

const unsigned char keys[16] =
{
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_r,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_f,
	SDLK_z,
	SDLK_x,
	SDLK_c,
	SDLK_v
};