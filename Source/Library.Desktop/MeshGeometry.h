#pragma once

#include "Asset.h"
#include "Factory.h"
#include "Renderer.h"

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

namespace FieaGameEngine
{
	class MeshGeometry final : public Asset
	{

		RTTI_DECLARATIONS(MeshGeometry, Asset)

	private:

		struct MeshVertex
		{
			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT2 Texcoord;
			DirectX::XMFLOAT3 Normal;
		};

	public:

		MeshGeometry();

		~MeshGeometry();

		virtual void Load(char* data) override;

		std::uint32_t GetFaces() const;

		ID3D11Buffer* GetVertexBuffer();

		const ID3D11Buffer* GetVertexBuffer() const;

		void SetRenderingState(Renderer* renderer);

	private:

		void GetAttributeCounts(char* pData,
			int& nNumVerts,
			int& nNumUVs,
			int& nNumNormals,
			int& nNumFaces);

		void GenerateVertexBuffer(int nNumFaces,
			float* pVertices,
			float* pUVs,
			float* pNormals,
			int*   pFaces,
			float* pVB);

		ID3D11Buffer* LoadOBJ(char* data,
			float** pVertexArray,
			unsigned int& nFaces,
			ID3D11Device* pDevice);

	private:

		ID3D11Buffer* mVertexBuffer;
		ID3D11InputLayout* mVertexLayout;

		std::uint32_t mFaces;
	};

	ConcreteFactory(Asset, MeshGeometry);
}