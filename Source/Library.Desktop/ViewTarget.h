#pragma once

#include "Camera.h"

namespace FieaGameEngine
{
	class ViewTarget
	{
	public:

		ViewTarget();
		~ViewTarget();

		void SetCamera(Camera* camera);

		void UpdateViewProjection(struct CBGlobal& globals);

	private:

		Camera* mCamera;
	};
}