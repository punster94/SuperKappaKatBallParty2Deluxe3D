#pragma once

#include "Renderable.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	class Mesh abstract : public Renderable
	{
		RTTI_DECLARATIONS(Mesh, Renderable)

	public:

		Mesh();

		Mesh(class Entity& owner);

		void SetOwner(Entity& owner);

		void SetVisible(bool visible);

		virtual void Render(Renderer* renderer) override = 0;

		void SetRelativePosition(glm::vec3 position);
		void SetRelativeRotation(glm::vec3 rotation);
		void SetRelativeScale(glm::vec3 scale);

		void AddRelativePosition(glm::vec3 deltaPosition);
		void AddRelativeRotation(glm::vec3 deltaRotation);
		void AddRelativeScale(glm::vec3 deltaScale);

	protected:

		class Entity* mOwner;

		bool mVisible;

		glm::vec3 mRelativePosition;
		glm::vec3 mRelativeRotation;
		glm::vec3 mRelativeScale;
	};
}