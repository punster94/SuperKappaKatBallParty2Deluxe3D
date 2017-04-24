#pragma once

#include "Entity.h"
#include "Mesh.h"

namespace FieaGameEngine
{
	class Actor : public Entity
	{
		RTTI_DECLARATIONS(Actor, Entity)

	public:

		Actor();

		~Actor();

		void SetPosition(glm::vec3 position);
		void SetRotation(glm::vec3 rotation);
		void SetScale(glm::vec3 scale);

		glm::vec3 GetPosition() const;
		glm::vec3 GetRotation() const;
		glm::vec3 GetScale() const;

		virtual void Update(WorldState& worldState) override;

	protected:

		void InitializeSignatures();

		glm::vec3 mPosition;
		glm::vec3 mRotation;
		glm::vec3 mScale;

	};

	ConcreteEntityFactory(Actor);
}