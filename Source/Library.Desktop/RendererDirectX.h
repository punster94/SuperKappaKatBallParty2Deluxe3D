#pragma once

#include <Windows.h>
#include <d3d11.h>
#include "Renderer.h"

namespace FieaGameEngine
{
	class RendererDirectX final : public Renderer
	{
	public:
		/**
		*	Initializes the renderer with the configuration.
		*/
		RendererDirectX(RenderConfiguration& config);

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

	private:
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
	};
}