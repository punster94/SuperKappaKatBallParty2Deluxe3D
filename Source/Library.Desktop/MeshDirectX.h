#pragma once

#include "Mesh.h"
#include <d3d11.h>

namespace FieaGameEngine
{
	class MeshDirectX : public Mesh
	{
	public:
		MeshDirectX(Renderer& renderer, const std::string& meshPath, const std::string& vertexShaderPath, const std::string& pixelShaderPath);
		~MeshDirectX();

		virtual void Render(Renderer& renderer) override;

	private:
		ID3D11VertexShader* mVertexShader;
		ID3D11InputLayout* mVertexLayout;
		ID3D11PixelShader* mPixelShader;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		glm::vec3* mVertices;
		std::uint32_t* mIndices;
		std::uint32_t mNumVertices;
		std::uint32_t mNumIndices;
	};
}