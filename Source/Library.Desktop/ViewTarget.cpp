#include "pch.h"
#include "ViewTarget.h"
#include <DirectXMath.h>

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

	void ViewTarget::UpdateViewProjection()
	{
		if (mCamera != nullptr)
		{
			const CameraSettings& settings = mCamera->GetCameraSettings();

			settings;
			//XMMATRIX viewMatrix;

		}
	}
}