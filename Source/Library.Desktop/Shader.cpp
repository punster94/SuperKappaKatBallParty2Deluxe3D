#include "pch.h"
#include "Shader.h"
#include "RendererDirectX.h"

#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Shader)

	Shader::Shader() : 
		mVertexShader(nullptr),
		mPixelShader(nullptr)
	{

	}

	Shader::~Shader()
	{
		if (mVertexShader != nullptr)
		{
			mVertexShader->Release();
			mVertexShader = nullptr;
		}

		if (mPixelShader != nullptr)
		{
			mPixelShader->Release();
			mPixelShader = nullptr;
		}
	}

	void Shader::Load(char* data)
	{
		data;
	}

	void Shader::SetRenderingState(Renderer* renderer)
	{
		if (mVertexShader != nullptr &&
			mPixelShader != nullptr)
		{
			RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(renderer);

			directX->Context()->VSSetShader(mVertexShader, nullptr, 0);
			directX->Context()->PSSetShader(mPixelShader, nullptr, 0);
		}
	}
}