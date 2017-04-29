#pragma once

#include <d3d11.h>
#include "MeshGeometry.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Entity.h"

namespace FieaGameEngine
{
	// Apparently CBuffers need to be 16byte alligned.
	// if mTextured changed to float, CreateBuffer fails to alloc
	// 20 bytes.

	struct CBQuad
	{
		DirectX::XMFLOAT4 mColor;
		DirectX::XMFLOAT4 mTextured;
	};

	class Quad : public Renderable
	{
	public:

		Quad();

		~Quad();

		virtual void Render(Renderer* renderer) override;

		void SetTexture(Texture* texture);

		void SetShaders(VertexShader* vertexShader,
						PixelShader* pixelShader);

		void SetRect(float x,
					 float y,
					 float width,
					 float height);

		void SetColor(glm::vec4 color);
		void SetColor(float r, float g, float b, float a);

	private:

		void UpdateInputLayout();

		void UpdateVertexBuffer();

		void UpdateCBuffer();

	private:

		Texture* mTexture;
		
		ID3D11InputLayout* mInputLayout;

		ID3D11Buffer* mConstantBuffer;
		ID3D11Buffer* mVertexBuffer;

		VertexShader* mVertexShader;
		PixelShader* mPixelShader;

		float mX;
		float mY;
		float mWidth;
		float mHeight;

		glm::vec4 mColor;
	};
}