#include "pch.h"
#include "Utils.h"

namespace FieaGameEngine
{
	char* DesktopUtils::ReadFile(const std::string& path, std::uint32_t& outFileSize)
	{
		char* data = nullptr;

		std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open())
		{
			// find the length of the file
			outFileSize = (std::uint32_t)file.tellg();

			// collect the file data
			data = new char[outFileSize];
			file.seekg(0, std::ios::beg);
			file.read(data, outFileSize);
			file.close();
		}

		return data;
	}
}