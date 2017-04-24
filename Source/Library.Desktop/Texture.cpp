#include "pch.h"
#include "Texture.h"
#include "Renderer.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Texture)

	Texture::Texture()
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::Load(char* data, std::uint32_t size)
	{
		data;
		size;
	}

	void Texture::SetRenderingState(Renderer* renderer)
	{
		renderer;
	}
}