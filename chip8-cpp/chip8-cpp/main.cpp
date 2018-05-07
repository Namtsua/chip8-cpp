#include "constants.hpp"
#include "chip8.hpp"
#include <stb\stb_image.h>
#include <thread>
#include <SDL/SDL_mixer.h>

Chip8 chip8;

using namespace std;
GLFWwindow* window;

int main(int argc, char* argv)
{

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW");
		return false;
	}

	// Get system resolution
	int w = 400;
	int h = 400;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Full screen & center
	window = glfwCreateWindow(w, h, "Chip 8 Emulator", nullptr, nullptr);

	if (window == nullptr)
	{
		fprintf(stderr, "Failed to create GLFW window");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // vsync

	// glad: load all OpenGL function pointers
	/*if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		return false;
	}*/

	chip8.initialize();
	if (!chip8.loadGame("./roms/tetris.rom"))
	{
		fprintf(stderr, "Failed to load ROM");
		return EXIT_FAILURE;
	}

	// render loop
	while (!glfwWindowShouldClose(window)) 
	{
		// poll IO events (keys pressed/released, mouse moved etc.)
		glfwPollEvents();
		chip8.emulateCycle();

		//if (chip8.getDrawFlag()) 
		chip8.setKeys();
	}

	// terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();

	return EXIT_SUCCESS;
}