#pragma once

#include "Renderable.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	enum RenderPass
	{
		RENDER_PASS_1 = 0,
		RENDER_PASS_2 = 1
	};

	class Mesh abstract : public Renderable
	{
		RTTI_DECLARATIONS(Mesh, Renderable)

	public:

		Mesh();

		Mesh(class Entity& owner);

		void SetOwner(Entity& owner);

		virtual void Render(Renderer* renderer) override = 0;

		void SetRelativePosition(glm::vec3 position);
		void SetRelativeRotation(glm::vec3 rotation);
		void SetRelativeScale(glm::vec3 scale);

		void AddRelativePosition(glm::vec3 deltaPosition);
		void AddRelativeRotation(glm::vec3 deltaRotation);
		void AddRelativeScale(glm::vec3 deltaScale);

		std::int32_t GetRenderPass();
		void SetRenderPass(int32_t renderPass);

	protected:

		class Entity* mOwner;

		glm::vec3 mRelativePosition;
		glm::vec3 mRelativeRotation;
		glm::vec3 mRelativeScale;

		std::int32_t mRenderPass;
	};
}