#include "pch.h"
#include "Game.h"
#include "RendererOpenGL.h"
#include "RenderConfigurationOpenGL.h"

UINT windowWidth = 720;
UINT windowHeight = 480;
std::string windowName = "Super Kappa Kat Ball Party 2 Deluxe 3D";

using namespace KatBall;
using namespace FieaGameEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	RenderConfigurationOpenGL config;
	config.windowWidth = windowWidth;
	config.windowHeight = windowHeight;
	config.windowName = windowName;

	RendererOpenGL renderer(config);
	
	Game game(renderer);
	game.Run();

	return 0;
}
