#include "pch.h"
#include "PixelShader.h"
#include "RendererDirectX.h"

#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(PixelShader)

		PixelShader::PixelShader() :
		mPixelShader(nullptr)
	{

	}

	PixelShader::~PixelShader()
	{
		if (mPixelShader != nullptr)
		{
			mPixelShader->Release();
			mPixelShader = nullptr;
		}
	}

	void PixelShader::Load(char* data, std::uint32_t size)
	{
		data;
		size;
	}

	void PixelShader::SetRenderingState(Renderer* renderer)
	{
		if (mPixelShader != nullptr)
		{
			RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(renderer);

			directX->Context()->PSSetShader(mPixelShader, nullptr, 0);
		}
	}
}