#pragma once

#include "Mesh.h"
#include <d3d11.h>
#include "MeshGeometry.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Actor.h"

namespace FieaGameEngine
{
	class MeshDirectX : public Mesh
	{

		RTTI_DECLARATIONS(MeshDirectX, Mesh)

	public:
		MeshDirectX();

		~MeshDirectX();

		virtual void Render(Renderer* renderer) override;

		void SetMeshGeometry(MeshGeometry* geometry);

		void SetShaders(VertexShader* vertexShader,
						PixelShader* pixelShader);

		void SetTexture(Texture* texture);

	private:

		MeshGeometry* mMeshGeometry;
		VertexShader* mVertexShader;
		PixelShader* mPixelShader;
		Texture* mTexture;

		ID3D11InputLayout* mInputLayout;
	};
}