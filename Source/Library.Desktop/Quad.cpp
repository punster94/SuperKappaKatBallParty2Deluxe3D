#include "pch.h"
#include "Quad.h"
#include "RendererDirectX.h"

#define DEFAULT_QUAD_WIDTH 0.1f
#define DEFAULT_QUAD_HEIGHT 0.1f
#define QUAD_VERTEX_BUFFER_SIZE 16 * 4
#define QUAD_VERTEX_SIZE 4 * 4
#define QUAD_NUM_VERTICES 4

namespace FieaGameEngine
{
	Quad::Quad() :
		mTexture(nullptr),
		mInputLayout(nullptr),
		mConstantBuffer(nullptr),
		mVertexBuffer(nullptr),
		mVertexShader(nullptr),
		mPixelShader(nullptr),
		mX(0.0f),
		mY(0.0f),
		mWidth(DEFAULT_QUAD_WIDTH),
		mHeight(DEFAULT_QUAD_HEIGHT),
		mColor(1.0f, 1.0f, 1.0f, 1.0f)
	{
		RendererDirectX* directX = RendererDirectX::Get();
		assert(directX != nullptr);

		// Create the constant buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CBQuad);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HRESULT hr = directX->Device()->CreateBuffer(&bd, nullptr, &mConstantBuffer);
		hr;

		// Create the D3D buffer object
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = QUAD_VERTEX_BUFFER_SIZE;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		directX->Device()->CreateBuffer(&bd, nullptr, &mVertexBuffer);

		// Upload default values to gpu
		UpdateVertexBuffer();
		UpdateCBuffer();
	}

	Quad::~Quad()
	{
		if (mInputLayout != nullptr)
		{
			mInputLayout->Release();
			mInputLayout = nullptr;
		}

		if (mConstantBuffer != nullptr)
		{
			mConstantBuffer->Release();
			mConstantBuffer = nullptr;
		}

		if (mVertexBuffer != nullptr)
		{
			mVertexBuffer->Release();
			mVertexBuffer = nullptr;
		}
	}

	void Quad::Render(Renderer* renderer)
	{
		if (mVisible)
		{
			assert(mVertexShader != nullptr);
			assert(mPixelShader != nullptr);
			assert(mVertexBuffer != nullptr);
			assert(mInputLayout != nullptr);

			mVertexShader->SetRenderingState(renderer);
			mPixelShader->SetRenderingState(renderer);


			static const UINT stride = QUAD_VERTEX_SIZE;
			static const UINT offset = 0U;

			RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(renderer);

			directX->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			directX->Context()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

			if (mTexture != nullptr)
			{
				mTexture->SetRenderingState(renderer);
			}

			directX->Context()->IASetInputLayout(mInputLayout);
			directX->Context()->PSSetConstantBuffers(0, 1, &mConstantBuffer);

			directX->Context()->Draw(QUAD_NUM_VERTICES, 0U);
		}
	}

	void Quad::SetTexture(Texture* texture)
	{
		mTexture = texture;
		UpdateCBuffer();
	}

	void Quad::SetShaders(VertexShader* vertexShader,
						  PixelShader* pixelShader)
	{
		mVertexShader = vertexShader;
		mPixelShader = pixelShader;
		UpdateInputLayout();
	}

	void Quad::UpdateCBuffer()
	{
		RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(Renderer::Get());
		assert(directX != nullptr);

		CBQuad cb;
		cb.mColor.x = mColor[0];
		cb.mColor.y = mColor[1];
		cb.mColor.z = mColor[2];
		cb.mColor.w = mColor[3];

		cb.mTextured.x = (mTexture == nullptr) ? 0.0f : 1.0f;

		directX->Context()->UpdateSubresource(mConstantBuffer, 0, nullptr, &cb, 0, 0);
	}

	void Quad::SetColor(glm::vec4 color)
	{
		mColor = color;
		UpdateCBuffer();

	}

	void Quad::SetColor(float r, float g, float b, float a)
	{
		mColor = glm::vec4(r, g, b, a);
		UpdateCBuffer();
	}

	void Quad::SetRect(float x,
					   float y,
					   float width,
					   float height)
	{
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;

		UpdateVertexBuffer();
	}

	void Quad::UpdateInputLayout()
	{
		RendererDirectX* directX = RendererDirectX::Get();
		assert(directX != nullptr);

		// About to recreate the input layout object. So delete any pre-existing layout
		if (mInputLayout != nullptr)
		{
			mInputLayout->Release();
			mInputLayout = nullptr;
		}

		// Define the input layout for vertices
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		HRESULT hr = directX->Device()->CreateInputLayout(layout, numElements, mVertexShader->GetBinaryData(),
			mVertexShader->GetBinarySize(), &mInputLayout);

		if (FAILED(hr))
		{
			printf("Failed to bind input layout. Possible attribute mismatch.\n");
			mVertexShader = nullptr;
			mPixelShader = nullptr;
		}
	}

	void Quad::UpdateVertexBuffer()
	{
		RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(Renderer::Get());
		float buffer[QUAD_VERTEX_BUFFER_SIZE];

		/*
			Triangle Strip format - 2 floats for x,y  2 floats for u,v
			4   12
			| \ |
			0   8
		*/
		buffer[0] = mX;
		buffer[1] = mY;
		buffer[2] = 0.0f;
		buffer[3] = 0.0f;

		buffer[4] = mX;
		buffer[5] = mY + mHeight;
		buffer[6] = 0.0f;
		buffer[7] = 1.0f;

		buffer[8] = mX + mWidth;
		buffer[9] = mY;
		buffer[10] = 1.0f;
		buffer[11] = 0.0f;

		buffer[12] = mX + mWidth;
		buffer[13] = mY + mHeight;
		buffer[14] = 1.0f;
		buffer[15] = 1.0f;

		directX->Context()->UpdateSubresource(mVertexBuffer, 0, nullptr, buffer, 0, 0);
	}
}