#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Renderer.h"
#include "ViewTarget.h"

namespace FieaGameEngine
{
	struct CBGlobal
	{
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};

	class RendererDirectX final : public Renderer
	{
	public:

		/**
		*	Creates a singleton.
		*/
		static RendererDirectX* Create(RenderConfiguration& config);

		/**
		*	Gets the singleton
		*/
		static RendererDirectX* Get();

		/**
		*	Destroys the singleton
		*/
		static void Destroy();

		/**
		*	Initializes the window and DirectX for rendering.
		*/
		virtual void Init() override;

		/**
		*	Prepares the scene for rendering.
		*/
		virtual void InitRenderFrame() override;

		/**
		*	Draws the scene to the render target.
		*/
		virtual void EndRenderFrame() override;

		/**
		*	Returns true if the window hasn't received a quit message.
		*/
		virtual bool IsValid() override;

		/**
		*	Releases DirectX memory.
		*/
		virtual void Shutdown() override;

		/**
		*	Gets the DirectX device.
		*/
		ID3D11Device* Device();

		/**
		*	Gets the DirectX device immediate context.
		*/
		ID3D11DeviceContext* Context();

		void SetCamera(Camera* camera) override;

		ID3D11Buffer* GetGlobalCBuffer();

		void AddViewRenderable(Renderable& renderable) override;

		void RemoveViewRenderable(Renderable& renderable) override;

		virtual void Render(World& world) override;

	private:

		/**
		*	Initializes the renderer with the configuration.
		*/
		RendererDirectX(RenderConfiguration& config);

		~RendererDirectX() = default;

		void InitWindow();
		void InitDirectX();

		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		HWND mWindow;
		MSG mMessage;
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		IDXGISwapChain* mSwapChain;
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		ID3D11DepthStencilState* mDepthStencilState;
		ID3D11RasterizerState* mRasterizerState;

		ID3D11BlendState* mBlendState;

		ID3D11Buffer* mGlobalCB;

		ViewTarget mViewTarget;
	};
}