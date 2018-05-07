#include "constants.hpp"
#include "chip8.hpp"

Chip8 chip8;

using namespace std;
const int& window_height = 512;
const int& window_width = 1024;

int main(int argc, char* argv)
{

	// SDL Window
	SDL_Window* window = NULL;

	// SDL Renderer
	SDL_Renderer* renderer = NULL;

	// Texture for the window
	SDL_Texture* texture = NULL;;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "Failed to initialize SDL");
		exit(1);
	}

	window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		fprintf(stderr, "SDL window could not be created!");
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		fprintf(stderr, "SDL renderer could not be created!");
		exit(1);
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT);
	if (renderer == NULL)
	{
		fprintf(stderr, "SDL texture could not be created!");
		exit(1);
	}
	

	chip8.initialize();
	if (!chip8.loadGame("./roms/tetris.rom"))
	{
		fprintf(stderr, "Failed to load ROM");
		return EXIT_FAILURE;
	}

	//// render loop
	//while (!glfwWindowShouldClose(window)) 
	//{
	//	// poll IO events (keys pressed/released, mouse moved etc.)
	//	glfwPollEvents();
	//	chip8.emulateCycle();

	//	//if (chip8.getDrawFlag()) 
	//	chip8.setKeys();
	//}

	while (true)
	{
		chip8.emulateCycle();
		if (chip8.getDrawFlag())
		{
			// do stuff
		}
		chip8.setKeys();
	}

	SDL_Quit();

	return EXIT_SUCCESS;
}