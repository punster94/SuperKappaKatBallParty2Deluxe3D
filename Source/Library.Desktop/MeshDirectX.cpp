#include "pch.h"
#include "MeshDirectX.h"
#include "RendererDirectX.h"
#include "Utils.h"

namespace FieaGameEngine
{
	MeshDirectX::MeshDirectX(Renderer& renderer, const std::string& meshPath, const std::string& vertexShaderPath, const std::string& pixelShaderPath)
	{
		RendererDirectX& directX = reinterpret_cast<RendererDirectX&>(renderer);

		// ignore mesh path for testing
		meshPath;
		mVertices = new glm::vec3[3];
		mIndices = new std::uint32_t[3];
		mNumIndices = 3;
		mNumIndices = 3;

		// create vertex shader
		std::uint32_t fileSize = 0;
		char* fileData = DesktopUtils::ReadFile(vertexShaderPath, fileSize);
		directX.Device()->CreateVertexShader(fileData, fileSize, nullptr, &mVertexShader);
		delete fileData;

		D3D11_INPUT_ELEMENT_DESC vertexInputs[1];

		vertexInputs[0] = { 0 };
		vertexInputs[0].SemanticName = "POSITION";
		vertexInputs[0].SemanticIndex = 0;
		vertexInputs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexInputs[0].InputSlot = 0;
		vertexInputs[0].AlignedByteOffset = 0;
		vertexInputs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		vertexInputs[0].InstanceDataStepRate = 0;

		directX.Device()->CreateInputLayout(vertexInputs, 1, fileData, fileSize, &mVertexLayout);

		// create pixel shader
		fileData = DesktopUtils::ReadFile(pixelShaderPath, fileSize);
		directX.Device()->CreatePixelShader(fileData, fileSize, nullptr, &mPixelShader);
		delete fileData;

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// create vertex buffer
		bufferDesc.ByteWidth = sizeof(glm::vec3) * mNumVertices;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		directX.Device()->CreateBuffer(&bufferDesc, nullptr, &mVertexBuffer);

		// create index buffer
		bufferDesc.ByteWidth = sizeof(std::uint32_t) * mNumIndices;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		directX.Device()->CreateBuffer(&bufferDesc, nullptr, &mIndexBuffer);
	}

	MeshDirectX::~MeshDirectX()
	{
		mVertexBuffer->Release();
		mVertexLayout->Release();
		mIndexBuffer->Release();
		mVertexShader->Release();
		mPixelShader->Release();
		delete[] mVertices;
		delete[] mIndices;
	}

	void MeshDirectX::Render(Renderer& renderer)
	{
		RendererDirectX& directX = reinterpret_cast<RendererDirectX&>(renderer);

		UINT stride = sizeof(glm::vec3);
		UINT offset = 0;

		// set shaders
		directX.Context()->VSSetShader(mVertexShader, nullptr, 0);
		directX.Context()->PSSetShader(mPixelShader, nullptr, 0);
		directX.Context()->IASetInputLayout(mVertexLayout);

		// TODO: Something along these lines for updating cbuffer on render...
		//D3D11_MAPPED_SUBRESOURCE mapResource;
		//context.Map(graphics.objectCBuffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
		//ObjectCBuffer* objectCBufferData = reinterpret_cast<ObjectCBuffer*>(mapResource.pData);
		//DirectX::XMStoreFloat4x4(&objectCBufferData->MVPMatrix, XMMatrixMultiplyTranspose(Transform::MMatrix(transform), vpMatrix));
		//context.Unmap(graphics.objectCBuffer.buffer, 0);
		//context.VSSetConstantBuffers(1, 1, &graphics.objectCBuffer.buffer);

		// draw mesh
		directX.Context()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		directX.Context()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		directX.Context()->DrawIndexed(mNumIndices, 0, 0);
	}
}