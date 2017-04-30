#include "pch.h"
#include "ViewTarget.h"
#include <DirectXMath.h>
#include "RendererDirectX.h"

using namespace DirectX;

namespace FieaGameEngine
{
	ViewTarget::ViewTarget()
	{

	}

	ViewTarget::~ViewTarget()
	{

	}

	void ViewTarget::SetCamera(Camera* camera)
	{
		mCamera = camera;
	}

	void ViewTarget::UpdateViewProjection(CBGlobal& globals)
	{
		if (mCamera != nullptr)
		{
			// Update view matrix based on camera position + rotation.
			XMMATRIX viewMatrix;

			glm::vec3 rotation = mCamera->GetRotation();
			glm::vec3 translation = mCamera->GetPosition();

			viewMatrix = XMMatrixTranslation(-translation.x, -translation.y, -translation.z);

			viewMatrix *= XMMatrixRotationY(-rotation.y);
			viewMatrix *= XMMatrixRotationX(-rotation.x);
			viewMatrix *= XMMatrixRotationZ(-rotation.z);

			globals.mView = XMMatrixTranspose(viewMatrix);

			// Update projection matrix based on camera settings
			const CameraSettings& settings = mCamera->GetCameraSettings();

			XMMATRIX projectionMatrix;

			projectionMatrix = XMMatrixPerspectiveLH(settings.mRight - settings.mLeft,
				settings.mTop - settings.mBottom,
				settings.mNear,
				settings.mFar);

			globals.mProjection = XMMatrixTranspose(projectionMatrix);
		}
	}

	void ViewTarget::AddRenderable(Renderable& renderable)
	{
		mRenderables.PushBack(&renderable);
	}

	void ViewTarget::RemoveRenderable(Renderable& renderable)
	{
		mRenderables.Remove(&renderable);
	}

	void ViewTarget::Render(Renderer* renderer) const
	{
		renderer->SetDepthMode(DepthMode::MODE_UI);

		for (Renderable* renderable : mRenderables)
		{
			renderable->Render(renderer);
		}

		renderer->SetDepthMode(DepthMode::MODE_OPAQUE);
	}
}