#pragma once

#include <Windows.h>
#include <string>
#include "RenderConfiguration.h"

namespace FieaGameEngine
{
	struct RenderConfigurationDirectX : public RenderConfiguration
	{
		HINSTANCE hInstance;
		int showCommand;
		std::wstring windowName;
	};
}