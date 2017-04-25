#include "pch.h"
#include "Game.h"
#include "RenderConfigurationDirectX.h"
#include "RendererDirectX.h"

UINT windowWidth = 1280;
UINT windowHeight = 720;
std::wstring windowName = L"Super Kappa Kat Ball Party 2 Deluxe 3D";

using namespace KatBall;
using namespace FieaGameEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);

	RenderConfigurationDirectX config;
	config.hInstance = instance;
	config.showCommand = showCommand;
	config.windowWidth = windowWidth;
	config.windowHeight = windowHeight;
	config.windowName = windowName;

	RendererDirectX* renderer = RendererDirectX::Create(config);

	Game game(*renderer);
	game.Run();

	return 0;
}