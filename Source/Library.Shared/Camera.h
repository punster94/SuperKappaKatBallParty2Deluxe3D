#pragma once

#include "Entity.h"

namespace FieaGameEngine
{
	enum class ProjectionMode
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	struct CameraSettings
	{
		float mLeft;
		float mRight;
		float mTop;
		float mBottom;
		float mNear;
		float mFar;
	};

	class Camera : public Entity
	{
	public:

		Camera();
		~Camera();

		void SetCameraSettings(const CameraSettings& settings);

		const CameraSettings& GetCameraSettings() const;

		void SetProjectionMode(ProjectionMode mode);

		ProjectionMode GetProjectionMode() const;

	protected:

		void InitializeSignatures();

		ProjectionMode mProjectionMode;

		CameraSettings mSettings;
	};
}
