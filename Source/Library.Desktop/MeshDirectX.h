#pragma once

#include "Mesh.h"
#include <d3d11.h>
#include "MeshGeometry.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"

namespace FieaGameEngine
{
	class MeshDirectX : public Mesh
	{

		RTTI_DECLARATIONS(MeshDirectX, Mesh)

	public:
		MeshDirectX();
		MeshDirectX(Actor& actor,
					MeshGeometry* geometry,
					Shader* shader = nullptr,
					Texture* texture = nullptr);

		~MeshDirectX();

		virtual void Render(Renderer* renderer) override;

		void SetMeshGeometry(MeshGeometry* geometry);

		void SetShader(Shader* shader);

		void SetTexture(Texture* texture);

	private:

		MeshGeometry* mMeshGeometry;
		Shader* mShader;
		Texture* mTexture;
	};
}