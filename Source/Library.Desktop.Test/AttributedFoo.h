#pragma once

#include "Attributed.h"

namespace TestingUtilities
{
	class AttributedFoo : public FieaGameEngine::Attributed
	{

		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed)

	private:

		static const std::uint32_t ArraySize = 5;

	public:

		AttributedFoo();

		AttributedFoo(const AttributedFoo& otherAttributedFoo);

		AttributedFoo& operator=(const AttributedFoo& otherAttributedFoo);

		AttributedFoo(AttributedFoo&& previousAttributedFoo);

		AttributedFoo& operator=(AttributedFoo&& previousAttributedFoo);

		virtual FieaGameEngine::Scope* Copy() const override;

	protected:

		void InitializeSignatures();

		virtual void FixExternalAttributes();

	private:

		void CopyPrivateDataMembers(const AttributedFoo& otherAttributedFoo);

		std::int32_t mIntAttribute;
		std::int32_t mIntAttributeArray[ArraySize];

		float mFloatAttribute;
		float mFloatAttributeArray[ArraySize];

		glm::vec4 mVecAttribute;
		glm::vec4 mVecAttributeArray[ArraySize];

		glm::mat4 mMatAttribute;
		glm::mat4 mMatAttributeArray[ArraySize];

		std::string mStringAttribute;
		std::string mStringAttributeArray[ArraySize];
	};
}