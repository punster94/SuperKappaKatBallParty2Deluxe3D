#include "pch.h"

namespace TestingUtilities
{
	RTTI_DEFINITIONS(AttributedFoo)

	AttributedFoo::AttributedFoo() :
		Attributed()
	{
		InitializeSignatures();
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& otherAttributedFoo) :
		FieaGameEngine::Attributed(otherAttributedFoo)
	{
		CopyPrivateDataMembers(otherAttributedFoo);
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& otherAttributedFoo)
	{
		if (this != &otherAttributedFoo)
		{
			FieaGameEngine::Attributed::operator=(otherAttributedFoo);

			CopyPrivateDataMembers(otherAttributedFoo);
		}

		return *this;
	}

	AttributedFoo::AttributedFoo(AttributedFoo&& previousAttributedFoo) :
		Attributed(std::move(previousAttributedFoo)), mIntAttribute(previousAttributedFoo.mIntAttribute),
		mFloatAttribute(previousAttributedFoo.mFloatAttribute), mVecAttribute(previousAttributedFoo.mVecAttribute),
		mMatAttribute(previousAttributedFoo.mMatAttribute), mStringAttribute(previousAttributedFoo.mStringAttribute)
	{
		CopyPrivateDataMembers(previousAttributedFoo);
	}

	AttributedFoo& AttributedFoo::operator=(AttributedFoo&& previousAttributedFoo)
	{
		if (this != &previousAttributedFoo)
		{
			Attributed::operator=(std::move(previousAttributedFoo));

			CopyPrivateDataMembers(previousAttributedFoo);
		}

		return *this;
	}

	FieaGameEngine::Scope* AttributedFoo::Copy() const
	{
		return new AttributedFoo(*this);
	}

	void AttributedFoo::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		glm::vec4 vector;
		glm::mat4 matrix;
		std::string string;

		AddInternalAttribute("Int", 1, 0);
		AddInternalAttribute("Float", 1, 0.0f);
		AddInternalAttribute("Vector", 1, vector);
		AddInternalAttribute("Matrix", 1, matrix);
		AddInternalAttribute("String", 1, string);

		AddExternalAttribute("ExternalInt", &mIntAttribute, 1);
		AddExternalAttribute("ExternalFloat", &mFloatAttribute, 1);
		AddExternalAttribute("ExternalVector", &mVecAttribute, 1);
		AddExternalAttribute("ExternalMatrix", &mMatAttribute, 1);
		AddExternalAttribute("ExternalString", &mStringAttribute, 1);

		AddExternalAttribute("ExternalIntArray", mIntAttributeArray, ArraySize);
		AddExternalAttribute("ExternalFloatArray", mFloatAttributeArray, ArraySize);
		AddExternalAttribute("ExternalVectorArray", mVecAttributeArray, ArraySize);
		AddExternalAttribute("ExternalMatrixArray", mMatAttributeArray, ArraySize);
		AddExternalAttribute("ExternalStringArray", mStringAttributeArray, ArraySize);
	}

	void AttributedFoo::FixExternalAttributes()
	{
		Append("ExternalInt").SetStorage(&mIntAttribute, 1);
		Append("ExternalFloat").SetStorage(&mFloatAttribute, 1);
		Append("ExternalVector").SetStorage(&mVecAttribute, 1);
		Append("ExternalMatrix").SetStorage(&mMatAttribute, 1);
		Append("ExternalString").SetStorage(&mStringAttribute, 1);

		Append("ExternalIntArray").SetStorage(mIntAttributeArray, ArraySize);
		Append("ExternalFloatArray").SetStorage(mFloatAttributeArray, ArraySize);
		Append("ExternalVectorArray").SetStorage(mVecAttributeArray, ArraySize);
		Append("ExternalMatrixArray").SetStorage(mMatAttributeArray, ArraySize);
		Append("ExternalStringArray").SetStorage(mStringAttributeArray, ArraySize);
	}

	void AttributedFoo::CopyPrivateDataMembers(const AttributedFoo& otherAttributedFoo)
	{
		mIntAttribute = otherAttributedFoo.mIntAttribute;
		mFloatAttribute = otherAttributedFoo.mFloatAttribute;
		mVecAttribute = otherAttributedFoo.mVecAttribute;
		mMatAttribute = otherAttributedFoo.mMatAttribute;
		mStringAttribute = otherAttributedFoo.mStringAttribute;

		for (std::uint32_t i = 0; i < ArraySize; ++i)
		{
			mIntAttributeArray[i] = otherAttributedFoo.mIntAttributeArray[i];
			mFloatAttributeArray[i] = otherAttributedFoo.mFloatAttributeArray[i];
			mVecAttributeArray[i] = otherAttributedFoo.mVecAttributeArray[i];
			mMatAttributeArray[i] = otherAttributedFoo.mMatAttributeArray[i];
			mStringAttributeArray[i] = otherAttributedFoo.mStringAttributeArray[i];
		}

		FixExternalAttributes();
	}
}