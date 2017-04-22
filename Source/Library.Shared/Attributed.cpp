#include "pch.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed(std::uint32_t initialSize) : 
		Scope(initialSize)
	{
		InitializeSignatures();
	}

	Attributed::Attributed(const Attributed& otherAttributed) :
		Scope(otherAttributed)
	{
		Append(sAttributedThisKey) = this;
	}

	Attributed& Attributed::operator=(const Attributed& otherAttributed)
	{
		if (this != &otherAttributed)
		{
			Scope::operator=(otherAttributed);

			Append(sAttributedThisKey) = this;
		}

		return *this;
	}

	Attributed::Attributed(Attributed&& previousAttributed) :
		Scope(std::move(previousAttributed))
	{
		Append(sAttributedThisKey) = this;
	}

	Attributed& Attributed::operator=(Attributed&& previousAttributed)
	{
		if (this != &previousAttributed)
		{
			Scope::operator=(std::move(previousAttributed));

			Append(sAttributedThisKey) = this;
		}

		return *this;
	}

	bool Attributed::operator==(const Attributed& otherAttributed) const
	{
		bool equal = mMap.Size() == otherAttributed.mMap.Size();

		if (equal)
		{
			for (PairType pair : otherAttributed.mMap)
			{
				Iterator it = mMap.Find(pair.first);

				if (it->first == sAttributedThisKey)
				{
					continue;
				}

				if (it == mMap.end() || it->second != pair.second)
				{
					equal = false;
					break;
				}
			}
		}

		return equal;
	}

	bool Attributed::operator!=(const Attributed& otherAttributed) const
	{
		return !(*this == otherAttributed);
	}

	bool Attributed::Equals(const RTTI* otherAttributed) const
	{
		if (this == otherAttributed)
		{
			return true;
		}

		if (otherAttributed == nullptr)
		{
			return false;
		}

		if (!otherAttributed->Is(TypeName()))
		{
			return false;
		}

		return *this == *(otherAttributed->As<Attributed>());
	}

	bool Attributed::IsPrescribedAttribute(const std::string& key) const
	{
		Vector<std::string>& prescribedAttributes = mPrescribedAttributeList[TypeIdInstance()];
		return IsAttribute(key) && prescribedAttributes.Find(key) != prescribedAttributes.end();
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& key) const
	{
		return IsAttribute(key) && !IsPrescribedAttribute(key);
	}

	bool Attributed::IsAttribute(const std::string& key) const
	{
		return Find(key) != nullptr;
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& key)
	{
		if (IsPrescribedAttribute(key))
		{
			throw std::exception("Cannot append auxiliary attribute using key of prescribed attribute");
		}

		return Append(key);
	}

	void Attributed::ClearPrescribedAttributes()
	{
		mPrescribedAttributeList.Clear();
	}

	void Attributed::CopyAuxiliaryAttributesInto(Attributed& otherAttributed) const
	{
		for (std::uint32_t i = mPrescribedAttributeList[TypeIdInstance()].Size(); i < mMap.Size(); ++i)
		{
			CopyEntryTo(i, otherAttributed);
		}
	}

	void Attributed::InitializeSignatures()
	{
		AddInternalAttribute(sAttributedThisKey, 1, this);
	}

	void Attributed::AddInternalAttribute(const std::string& key, const std::uint32_t size, std::int32_t initialValue)
	{
		Datum& datum = Append(key);
		datum.Clear();
		datum.SetType(Datum::DatumType::Integer);
		datum.Reserve(size);

		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}

		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddInternalAttribute(const std::string& key, const std::uint32_t size, float initialValue)
	{
		Datum& datum = Append(key);
		datum.Clear();
		datum.SetType(Datum::DatumType::Float);
		datum.Reserve(size);

		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}

		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddInternalAttribute(const std::string& key, const std::uint32_t size, glm::vec4& initialValue)
	{
		Datum& datum = Append(key);
		datum.Clear();
		datum.SetType(Datum::DatumType::Vec4);
		datum.Reserve(size);

		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}

		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddInternalAttribute(const std::string& key, const std::uint32_t size, glm::mat4& initialValue)
	{
		Datum& datum = Append(key);
		datum.Clear();
		datum.SetType(Datum::DatumType::Mat4);
		datum.Reserve(size);

		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}

		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddInternalAttribute(const std::string& key, const std::uint32_t size, std::string& initialValue)
	{
		Datum& datum = Append(key);
		datum.Clear();
		datum.SetType(Datum::DatumType::String);
		datum.Reserve(size);

		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}

		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddInternalAttribute(const std::string& key, const std::uint32_t size, RTTI* initialValue)
	{
		Datum& datum = Append(key);
		datum.Clear();
		datum.SetType(Datum::DatumType::Pointer);
		datum.Reserve(size);

		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}

		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddExternalAttribute(const std::string& key, std::int32_t* data, const std::uint32_t size)
	{
		Append(key).SetStorage(data, size);
		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddExternalAttribute(const std::string& key, float* data, const std::uint32_t size)
	{
		Append(key).SetStorage(data, size);
		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddExternalAttribute(const std::string& key, glm::vec4* data, const std::uint32_t size)
	{
		Append(key).SetStorage(data, size);
		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddExternalAttribute(const std::string& key, glm::mat4* data, const std::uint32_t size)
	{
		Append(key).SetStorage(data, size);
		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddExternalAttribute(const std::string& key, std::string* data, const std::uint32_t size)
	{
		Append(key).SetStorage(data, size);
		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddExternalAttribute(const std::string& key, RTTI** data, const std::uint32_t size)
	{
		Append(key).SetStorage(data, size);
		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddNestedScope(const std::string& key, Scope& scope)
	{
		Adopt(scope, key);
		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddEmptyTable(const std::string& key)
	{
		Datum& table = Append(key);
		table.Clear();
		table.SetType(Datum::DatumType::Table);

		AddPrescribedAttributeKey(key);
	}

	void Attributed::AddEmptyString(const std::string& key)
	{
		Datum& emptyString = Append(key);
		emptyString.Clear();
		emptyString.SetType(Datum::DatumType::String);

		AddPrescribedAttributeKey(key);
	}

	void Attributed::FixExternalAttributes()
	{

	}

	void Attributed::AddPrescribedAttributeKey(const std::string& key)
	{
		if (!IsPrescribedAttribute(key))
		{
			mPrescribedAttributeList[TypeIdInstance()].PushBack(key);
		}
	}

	HashMap<std::uint64_t, Vector<std::string>> Attributed::mPrescribedAttributeList;

	const std::string Attributed::sAttributedThisKey = "this";
}