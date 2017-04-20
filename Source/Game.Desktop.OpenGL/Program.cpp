#include "pch.h"
#include "Program.h"

GLFWwindow* window;

UINT windowWidth = 1920;
UINT windowHeight = 1080;

char* windowName = "OpenGL Game";

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	//FieaGameEngine::SList list(20);
	//std::int32_t data = list.Data();
	//data;

	InitializeWindow();

	while (!(glfwWindowShouldClose(window)))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void InitializeWindow()
{
	glfwInit();
	window = glfwCreateWindow(windowWidth, windowHeight, windowName, nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwInit();
}