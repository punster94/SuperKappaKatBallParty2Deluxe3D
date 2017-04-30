#include "pch.h"
#include "MeshDirectX.h"
#include "RendererDirectX.h"
#include "Utils.h"

using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(MeshDirectX)

	MeshDirectX::MeshDirectX() :
		mMeshGeometry(nullptr),
		mVertexShader(nullptr),
		mPixelShader(nullptr),
		mTexture(nullptr),
		mInputLayout(nullptr),
		mConstantBuffer(nullptr)
	{
		RendererDirectX* directX = RendererDirectX::Get();
		assert(directX != nullptr);

		// Create the constant buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CBMesh);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		directX->Device()->CreateBuffer(&bd, nullptr, &mConstantBuffer);
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

			// CBuffer updates + bindings
			directX->Context()->IASetInputLayout(mInputLayout);

			CBMesh meshConstants;
			UpdateWorldMatrix(meshConstants);

			directX->Context()->UpdateSubresource(mConstantBuffer, 0, nullptr, &meshConstants, 0, 0);

			ID3D11Buffer* cbuffers[2] = { directX->GetGlobalCBuffer(), mConstantBuffer };
			directX->Context()->VSSetConstantBuffers(0, 2, cbuffers);

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

	void MeshDirectX::UpdateWorldMatrix(CBMesh& meshConstants)
	{
		assert(mOwner != nullptr);

		XMMATRIX worldMatrix;

		glm::vec3 position = mOwner->GetPosition();
		glm::vec3 rotation = mOwner->GetRotation();
		glm::vec3 scale = mOwner->GetScale();

		position += mRelativePosition;

		worldMatrix = XMMatrixIdentity();

		worldMatrix = worldMatrix * XMMatrixRotationY(mRelativeRotation.y);
		worldMatrix = worldMatrix * XMMatrixRotationX(mRelativeRotation.x);
		worldMatrix = worldMatrix * XMMatrixRotationZ(mRelativeRotation.z);

		worldMatrix = worldMatrix * XMMatrixTranslation(position.x, position.y, position.z);

		worldMatrix = worldMatrix * XMMatrixRotationY(rotation.y);
		worldMatrix = worldMatrix * XMMatrixRotationX(rotation.x);
		worldMatrix = worldMatrix * XMMatrixRotationZ(rotation.z);

		worldMatrix = worldMatrix * XMMatrixScaling(scale.x, scale.y, scale.z);

		meshConstants.mWorld = XMMatrixTranspose(worldMatrix);
	}
}