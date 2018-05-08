#include "chip8.hpp"
#include "constants.hpp"
Chip8 chip8;

using namespace std;
const int& window_height = 512;
const int& window_width = 1024;
uint32_t temp_pixels[Constants::SCREEN_WIDTH * Constants::SCREEN_HEIGHT];
uint8_t keys_map[16] =
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

	renderer = SDL_CreateRenderer(window, -1, 0);
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
	if (!chip8.loadGame("./roms/pong.ch8"))
	{
		fprintf(stderr, "Failed to load ROM");
		return EXIT_FAILURE;
	}

	CPU& cpu = chip8.getCPU();

	while (true)
	{
		// Emulate a Chip8 CPU cycle
		chip8.emulateCycle();

		// Draw if we need to.
		if (chip8.getDrawFlag())
		{
			chip8.setDrawFlag(false);
			for (int i = 0; i < (Constants::SCREEN_WIDTH * Constants::SCREEN_HEIGHT); i++)
			{
				uint8_t pixel = cpu.gfx[i];
				temp_pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
			}
			SDL_UpdateTexture(texture, NULL, temp_pixels, 64 * sizeof(Uint32));
			// Clear renderer and screen
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			// Exit program if you closes the window
			if (e.type == SDL_QUIT) exit(1);

			// Handle when we press the key
			if (e.type == SDL_KEYDOWN)
			{
				// Exit program via ESC key
				if (e.key.keysym.sym == SDLK_ESCAPE) exit(1);

				for (int i = 0; i < Constants::KEYPAD_COUNT; i++)
				{
					// Check for any matching keys in our mapping
					if (e.key.keysym.sym == keys_map[i])
						cpu.key[i] = 1;
				}
			}

			// Handle when we lift the key
			if (e.type == SDL_KEYUP)
			{
				for (int i = 0; i < Constants::KEYPAD_COUNT; i++)
				{
					if (e.key.keysym.sym == keys_map[i])
						cpu.key[i] = 0;
				}
			}
		}


		// Sleep to slow down emulation speed
		std::this_thread::sleep_for(std::chrono::microseconds(5000));

	}

	SDL_Quit();

	return EXIT_SUCCESS;
}