#include "pch.h"
#include "Camera.h"

#define DEFAULT_WIDTH 0.05f
#define DEFAULT_HEIGHT 0.035f
#define DEFAULT_NEAR 0.1f
#define DEFAULT_FAR 1024.0f

namespace FieaGameEngine
{
	Camera::Camera() :
		mProjectionMode(ProjectionMode::PERSPECTIVE)
	{
		mSettings.mLeft = -DEFAULT_WIDTH;
		mSettings.mRight = DEFAULT_WIDTH;
		mSettings.mTop = DEFAULT_HEIGHT;
		mSettings.mBottom = -DEFAULT_HEIGHT;
		mSettings.mNear = DEFAULT_NEAR;
		mSettings.mFar = DEFAULT_FAR;
	}

	Camera::~Camera()
	{

	}

	void Camera::SetCameraSettings(const CameraSettings& settings)
	{
		mSettings = settings;
	}

	const CameraSettings& Camera::GetCameraSettings() const
	{
		return mSettings;
	}

	void Camera::SetProjectionMode(ProjectionMode mode)
	{
		mProjectionMode = mode;
	}

	ProjectionMode Camera::GetProjectionMode() const
	{
		return mProjectionMode;
	}

	void Camera::InitializeSignatures()
	{
		Entity::InitializeSignatures();
	}
}