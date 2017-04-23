#pragma once

#include "Mesh.h"

namespace FieaGameEngine
{
	class MeshOpenGL : public Mesh
	{
	public:
		virtual void Render(Renderer* renderer) override;
	};
}