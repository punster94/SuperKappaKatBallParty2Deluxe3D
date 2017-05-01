#pragma once

#include "Mesh.h"
#include <d3d11.h>
#include "MeshGeometry.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Entity.h"

namespace FieaGameEngine
{
	struct CBMesh
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMFLOAT4 mColor;
	};

	class MeshDirectX : public Mesh
	{

		RTTI_DECLARATIONS(MeshDirectX, Mesh)

	private:

		static D3D11_BLEND_DESC1* sBlendState;

	public:
		MeshDirectX();

		~MeshDirectX();

		virtual void Render(Renderer* renderer) override;

		void SetMeshGeometry(MeshGeometry* geometry);

		void SetShaders(VertexShader* vertexShader,
						PixelShader* pixelShader);

		void SetTexture(Texture* texture);

	private:

		void UpdateWorldMatrix(CBMesh& meshConstants);

		MeshGeometry* mMeshGeometry;
		VertexShader* mVertexShader;
		PixelShader* mPixelShader;
		Texture* mTexture;

		ID3D11InputLayout* mInputLayout;

		ID3D11Buffer* mConstantBuffer;
	};
}