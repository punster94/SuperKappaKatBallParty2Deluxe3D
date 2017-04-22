#pragma once

#include <string>
#include "RenderConfiguration.h"

namespace FieaGameEngine
{
	struct RenderConfigurationOpenGL : public RenderConfiguration
	{
		std::string windowName;
	};
}