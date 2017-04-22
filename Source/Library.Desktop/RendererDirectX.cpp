#include "pch.h"
#include "RendererDirectX.h"
#include "RenderConfigurationDirectX.h"
#include <DirectXColors.h>

namespace FieaGameEngine
{
	RendererDirectX::RendererDirectX(RenderConfiguration& config)
		: Renderer(config)
	{
	}

	void RendererDirectX::Init()
	{
		InitWindow();
		InitDirectX();
		ZeroMemory(&mMessage, sizeof(mMessage));
	}

	void RendererDirectX::InitWindow()
	{
		RenderConfigurationDirectX* config = reinterpret_cast<RenderConfigurationDirectX*>(mConfig);

		WNDCLASSEX window;
		ZeroMemory(&window, sizeof(window));
		window.cbSize = sizeof(WNDCLASSEX);
		window.style = CS_CLASSDC;

		window.lpfnWndProc = WndProc;
		window.hInstance = config->hInstance;
		window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		window.lpszClassName = L"WindowClass";

		RECT windowRectangle = { 0, 0, (LONG)config->windowWidth, (LONG)config->windowHeight };
		AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&window);
		mWindow = CreateWindowEx(0, L"WindowClass", config->windowName.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, config->hInstance, nullptr);

		ShowWindow(mWindow, config->showCommand);
		UpdateWindow(mWindow);
	}

	void RendererDirectX::InitDirectX()
	{
		RenderConfigurationDirectX* config = reinterpret_cast<RenderConfigurationDirectX*>(mConfig);

		DXGI_SWAP_CHAIN_DESC swapChainDescription;
		ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDescription.BufferCount = 1;
		swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.OutputWindow = mWindow;
		swapChainDescription.SampleDesc.Count = 4;
		swapChainDescription.Windowed = TRUE;

		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDescription, &mSwapChain, &mDevice, nullptr, &mDeviceContext);

		ID3D11Texture2D* backBuffer;
		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

		mDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
		backBuffer->Release();

		mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (FLOAT)config->windowWidth;
		viewport.Height = (FLOAT)config->windowHeight;

		mDeviceContext->RSSetViewports(1, &viewport);
	}

	void RendererDirectX::InitRenderFrame()
	{
		mDeviceContext->ClearRenderTargetView(mRenderTargetView, DirectX::Colors::Lavender);
		mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void RendererDirectX::EndRenderFrame()
	{
		mSwapChain->Present(0, 0);
	}

	bool RendererDirectX::IsValid()
	{
		if (PeekMessage(&mMessage, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&mMessage);
			DispatchMessage(&mMessage);
		}
		
		return (mMessage.message != WM_QUIT);
	}

	void RendererDirectX::Shutdown()
	{
		mSwapChain->Release();
		mDevice->Release();
		mDeviceContext->Release();
	}

	LRESULT WINAPI RendererDirectX::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_DESTROY)
		{
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}
}