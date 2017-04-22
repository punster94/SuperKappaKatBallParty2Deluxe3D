#pragma once

#include "Renderer.h"

namespace FieaGameEngine
{
	class RendererOpenGL : public Renderer
	{
	public:
		virtual void Init() override;
		virtual void InitRenderFrame() override;
	};
}