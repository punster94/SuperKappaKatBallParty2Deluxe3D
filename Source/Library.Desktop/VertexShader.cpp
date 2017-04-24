#include "pch.h"
#include "VertexShader.h"
#include "RendererDirectX.h"

#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(VertexShader)

		VertexShader::VertexShader() :
		mVertexShader(nullptr),
		mBinaryData(nullptr),
		mBinarySize(0U)
	{

	}

	VertexShader::~VertexShader()
	{
		if (mVertexShader != nullptr)
		{
			mVertexShader->Release();
			mVertexShader = nullptr;
		}
	}

	void VertexShader::Load(char* data, std::uint32_t size)
	{
		RendererDirectX* directX = RendererDirectX::Get();
		assert(directX != nullptr);

		// Release any previously loaded shader
		if (mVertexShader != nullptr)
		{
			mVertexShader->Release();
			mVertexShader = nullptr;

			assert(mBinaryData != nullptr);
			free(mBinaryData);
			mBinaryData = nullptr;
			mBinarySize = 0U;
		}

		directX->Device()->CreateVertexShader(data, size, nullptr, &mVertexShader);

		// Make a copy of the binary for use when creating input layouts
		mBinaryData = reinterpret_cast<char*>(malloc(size));
		memcpy(mBinaryData, data, size);
		mBinarySize = size;
	}

	void VertexShader::SetRenderingState(Renderer* renderer)
	{
		if (mVertexShader != nullptr)
		{
			RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(renderer);

			directX->Context()->VSSetShader(mVertexShader, nullptr, 0);
		}
	}

	const void* VertexShader::GetBinaryData() const
	{
		return mBinaryData;
	}

	std::uint32_t VertexShader::GetBinarySize() const
	{
		return mBinarySize;
	}
}