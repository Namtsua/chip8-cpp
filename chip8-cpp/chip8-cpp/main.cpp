#include "constants.hpp"
#include "chip8.hpp"

Chip8 chip8;

using namespace std;
const int& window_height = 512;
const int& window_width = 1024;
unsigned char temp_pixels[Constants::SCREEN_WIDTH * Constants::SCREEN_HEIGHT];

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
	SDL_RenderSetLogicalSize(renderer, window_width, window_height);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT);
	if (renderer == NULL)
	{
		fprintf(stderr, "SDL texture could not be created!");
		exit(1);
	}
	

	chip8.initialize();
	if (!chip8.loadGame("./roms/pong.rom"))
	{
		fprintf(stderr, "Failed to load ROM");
		return EXIT_FAILURE;
	}

	CPU& cpu = chip8.getCPU();

	while (true)
	{
		chip8.emulateCycle();

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT) exit(1);

			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE) exit(1);

				for (int i = 0; i < Constants::KEYPAD_COUNT; i++)
				{
					if (e.key.keysym.sym == keys[i])
					{
						cpu.key[i] = 1;
						break;
					}
				}
			}

			if (e.type == SDL_KEYUP)
			{
				for (int i = 0; i < Constants::KEYPAD_COUNT; i++)
				{
					if (e.key.keysym.sym == keys[i])
					{
						cpu.key[i] = 0;
						break;
					}
				}
			}
		}


		// Draw if we need to.
		if (chip8.getDrawFlag())
		{
			chip8.setDrawFlag(false);
			for (int i = 0; i < (Constants::SCREEN_WIDTH * Constants::SCREEN_HEIGHT); ++i)
			{
				unsigned char pixel = cpu.gfx[i];
				temp_pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
			}
			SDL_UpdateTexture(texture, NULL, temp_pixels, 64 * sizeof(Uint32));
			// Clear renderer and screen
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}


		// Sleep to slow down emulation speed
		std::this_thread::sleep_for(std::chrono::microseconds(1200));

		chip8.setKeys();
	}

	SDL_Quit();

	return EXIT_SUCCESS;
}