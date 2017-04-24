#include "pch.h"
#include "MeshDirectX.h"
#include "RendererDirectX.h"
#include "Utils.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(MeshDirectX)

		//MeshDirectX::MeshDirectX(Renderer* renderer, const std::string& meshPath, const std::string& vertexShaderPath, const std::string& pixelShaderPath)
		//{
		//	RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(renderer);

		//	// ignore mesh path for testing
		//	meshPath;
		//	mVertices = new glm::vec3[3];
		//	mIndices = new std::uint32_t[3];

		//	mVertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
		//	mVertices[1] = glm::vec3(0.0f, 1.0f, 0.0f);
		//	mVertices[2] = glm::vec3(1.0f, -1.0f, 0.0f);
		//	mIndices[0] = 0;
		//	mIndices[1] = 1;
		//	mIndices[2] = 2;

		//	mNumVertices = 3;
		//	mNumIndices = 3;

		//	//// create vertex shader
		//	std::uint32_t fileSize = 0;
		//	char* vertexFileData = DesktopUtils::ReadFile(vertexShaderPath, fileSize);
		//	directX->Device()->CreateVertexShader(vertexFileData, fileSize, nullptr, &mVertexShader);

		//	D3D11_INPUT_ELEMENT_DESC vertexInputs[1];

		//	vertexInputs[0] = { 0 };
		//	vertexInputs[0].SemanticName = "POSITION";
		//	vertexInputs[0].SemanticIndex = 0;
		//	vertexInputs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		//	vertexInputs[0].InputSlot = 0;
		//	vertexInputs[0].AlignedByteOffset = 0;
		//	vertexInputs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		//	vertexInputs[0].InstanceDataStepRate = 0;

		//	directX->Device()->CreateInputLayout(vertexInputs, 1, vertexFileData, fileSize, &mVertexLayout);
		//	delete vertexFileData;

		//	// create pixel shader
		//	char* pixelFileData = DesktopUtils::ReadFile(pixelShaderPath, fileSize);
		//	directX->Device()->CreatePixelShader(pixelFileData, fileSize, nullptr, &mPixelShader);
		//	delete pixelFileData;

		//	D3D11_BUFFER_DESC bufferDesc;
		//	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		//	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

		//	D3D11_SUBRESOURCE_DATA vertexData;
		//	vertexData.pSysMem = mVertices;
		//	vertexData.SysMemPitch = 0;
		//	vertexData.SysMemSlicePitch = 0;

		//	// create vertex buffer
		//	bufferDesc.ByteWidth = sizeof(glm::vec3) * mNumVertices;
		//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//	directX->Device()->CreateBuffer(&bufferDesc, &vertexData, &mVertexBuffer);

		//	D3D11_SUBRESOURCE_DATA indexData;
		//	indexData.pSysMem = mIndices;
		//	indexData.SysMemPitch = 0;
		//	indexData.SysMemSlicePitch = 0;

		//	// create index buffer
		//	bufferDesc.ByteWidth = sizeof(std::uint32_t) * mNumIndices;
		//	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//	directX->Device()->CreateBuffer(&bufferDesc, &indexData, &mIndexBuffer);
		//}

	MeshDirectX::MeshDirectX() :
		mMeshGeometry(nullptr),
		mVertexShader(nullptr),
		mPixelShader(nullptr),
		mTexture(nullptr),
		mInputLayout(nullptr)
	{

	}


	MeshDirectX::~MeshDirectX()
	{

	}

	void MeshDirectX::Render(Renderer* renderer)
	{
		if (mVisible)
		{
			assert(mMeshGeometry != nullptr);
			assert(mVertexShader != nullptr);
			assert(mPixelShader != nullptr);

			RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(renderer);

			mMeshGeometry->SetRenderingState(renderer);
			mVertexShader->SetRenderingState(renderer);
			mPixelShader->SetRenderingState(renderer);

			if (mTexture != nullptr)
			{
				mTexture->SetRenderingState(renderer);
			}

			directX->Context()->IASetInputLayout(mInputLayout);

			// TODO: Something along these lines for updating cbuffer on render...
			//D3D11_MAPPED_SUBRESOURCE mapResource;
			//context.Map(graphics.objectCBuffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
			//ObjectCBuffer* objectCBufferData = reinterpret_cast<ObjectCBuffer*>(mapResource.pData);
			//DirectX::XMStoreFloat4x4(&objectCBufferData->MVPMatrix, XMMatrixMultiplyTranspose(Transform::MMatrix(transform), vpMatrix));
			//context.Unmap(graphics.objectCBuffer.buffer, 0);
			//context.VSSetConstantBuffers(1, 1, &graphics.objectCBuffer.buffer);

			directX->Context()->Draw(mMeshGeometry->GetFaces() * 3, 0U);
		}
	}

	void MeshDirectX::SetMeshGeometry(MeshGeometry* geometry)
	{
		mMeshGeometry = geometry;
	}

	void MeshDirectX::SetShaders(VertexShader* vertexShader,
								 PixelShader* pixelShader)
	{
		RendererDirectX* directX = RendererDirectX::Get();
		assert(directX != nullptr);

		mVertexShader = vertexShader;
		mPixelShader = pixelShader;

		if (mVertexShader != nullptr &&
			mPixelShader != nullptr)
		{
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
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{  "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
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

	}

	void MeshDirectX::SetTexture(Texture* texture)
	{
		mTexture = texture;
	}
}