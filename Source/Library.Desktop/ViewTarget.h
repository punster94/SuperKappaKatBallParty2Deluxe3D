#pragma once

#include "Camera.h"
#include "Renderable.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class ViewTarget
	{
	public:

		ViewTarget();
		~ViewTarget();

		void SetCamera(Camera* camera);

		void UpdateViewProjection(struct CBGlobal& globals);

		void AddRenderable(Renderable& renderable);

		void RemoveRenderable(Renderable& renderable);

		void Render(Renderer* renderer) const;

	private:

		Vector<Renderable*> mRenderables;

		Camera* mCamera;
	};
}