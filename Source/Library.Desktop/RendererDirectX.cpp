#include "pch.h"
#include "RendererDirectX.h"
#include "RenderConfigurationDirectX.h"
#include "MeshDirectX.h"
#include <DirectXColors.h>

namespace FieaGameEngine
{
	RendererDirectX* RendererDirectX::Create(RenderConfiguration& config)
	{
		if (sInstance == nullptr)
		{
			sInstance = new RendererDirectX(config);
		}

		return static_cast<RendererDirectX*>(sInstance);
	}

	RendererDirectX* RendererDirectX::Get()
	{
		return static_cast<RendererDirectX*>(sInstance);
	}

	void RendererDirectX::Destroy()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	RendererDirectX::RendererDirectX(RenderConfiguration& config)
		: Renderer(config),
		  mBlendState(nullptr)
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
		swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.OutputWindow = mWindow;
		swapChainDescription.SampleDesc.Count = 1;
		swapChainDescription.SampleDesc.Quality = 0;
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
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		mDeviceContext->RSSetViewports(1, &viewport);

		D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };

		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		mDevice->CreateDepthStencilState(&dsDesc, &mDepthStencilStateDepthTest);
		mDeviceContext->OMSetDepthStencilState(mDepthStencilStateDepthTest, 1);

		dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		mDevice->CreateDepthStencilState(&dsDesc, &mDepthStencilStateNoDepthTest);

		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		mDevice->CreateDepthStencilState(&dsDesc, &mDepthStencilStateDepthTestNoWrite);

		D3D11_TEXTURE2D_DESC dsBufferDesc = { 0 };
		dsBufferDesc.Width = config->windowWidth;
		dsBufferDesc.Height = config->windowHeight;
		dsBufferDesc.MipLevels = 1;
		dsBufferDesc.ArraySize = 1;
		dsBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsBufferDesc.SampleDesc.Count = 1;
		dsBufferDesc.SampleDesc.Quality = 0;
		dsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsBufferDesc.CPUAccessFlags = 0;
		dsBufferDesc.MiscFlags = 0;

		mDevice->CreateTexture2D(&dsBufferDesc, nullptr, &backBuffer);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
		ZeroMemory(&dsViewDesc, sizeof(dsViewDesc));
		dsViewDesc.Format = dsBufferDesc.Format;
		dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsViewDesc.Texture2D.MipSlice = 0;
		mDevice->CreateDepthStencilView(backBuffer, &dsViewDesc, &mDepthStencilView);
		backBuffer->Release();
		mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

		D3D11_RASTERIZER_DESC rasterizerDesc;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		rasterizerDesc.FrontCounterClockwise = true;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.AntialiasedLineEnable = false;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		mDevice->CreateRasterizerState(&rasterizerDesc, &mRasterizerState);
		mDeviceContext->RSSetState(mRasterizerState);

		// Create the global CBuffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CBGlobal);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		mDevice->CreateBuffer(&bd, nullptr, &mGlobalCB);

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		mDevice->CreateBlendState(&blendDesc, &mBlendState);
		mDeviceContext->OMSetBlendState(mBlendState, nullptr, 0xffffffff);
	}

	void RendererDirectX::InitRenderFrame()
	{
		mDeviceContext->ClearRenderTargetView(mRenderTargetView, DirectX::Colors::Lavender);
		mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		CBGlobal globals;
		mViewTarget.UpdateViewProjection(globals);
		memcpy(&globals.mLightDirection, &mLightDirection, sizeof(mLightDirection));
		memcpy(&globals.mAmbientLight, &mAmbientLight, sizeof(mAmbientLight));

		mDeviceContext->UpdateSubresource(mGlobalCB, 0, nullptr, &globals, 0, 0);
	}

	void RendererDirectX::EndRenderFrame()
	{
		mSwapChain->Present(DXGI_SWAP_EFFECT_SEQUENTIAL, 0);
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

	void RendererDirectX::SetDepthMode(DepthMode mode)
	{
		switch (mode)
		{
		case DepthMode::MODE_OPAQUE:
			mDeviceContext->OMSetDepthStencilState(mDepthStencilStateDepthTest, 1);
			break;

		case DepthMode::MODE_UI:
			mDeviceContext->OMSetDepthStencilState(mDepthStencilStateNoDepthTest, 1);
			break;

		case DepthMode::MODE_TRANSLUCENT:
			mDeviceContext->OMSetDepthStencilState(mDepthStencilStateDepthTestNoWrite, 1);
			break;
		}
	}

	void RendererDirectX::Shutdown()
	{
		mRasterizerState->Release();
		mDepthStencilView->Release();
		mDepthStencilStateDepthTest->Release();
		mRenderTargetView->Release();

		mSwapChain->Release();
		mDevice->Release();
		mDeviceContext->Release();
	}

	ID3D11Device* RendererDirectX::Device()
	{
		return mDevice;
	}

	ID3D11DeviceContext* RendererDirectX::Context()
	{
		return mDeviceContext;
	}

	void RendererDirectX::SetCamera(Camera* camera)
	{
		mViewTarget.SetCamera(camera);
	}

	ID3D11Buffer* RendererDirectX::GetGlobalCBuffer()
	{
		return mGlobalCB;
	}

	void RendererDirectX::AddViewRenderable(Renderable& renderable)
	{
		mViewTarget.AddRenderable(renderable);
	}

	void RendererDirectX::RemoveViewRenderable(Renderable& renderable)
	{
		mViewTarget.RemoveRenderable(renderable);
	}

	void RendererDirectX::Render(World& world)
	{
		Renderer::Render(world);
		
		mViewTarget.Render(this);
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