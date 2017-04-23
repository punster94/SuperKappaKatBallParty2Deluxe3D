#pragma once

#include <string>

namespace FieaGameEngine
{
	class DesktopUtils abstract
	{
	public:
		static char* ReadFile(const std::string& path, std::uint32_t& outFileSize);
	};
}