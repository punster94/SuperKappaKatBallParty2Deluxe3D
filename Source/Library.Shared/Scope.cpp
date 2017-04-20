#include "pch.h"

namespace FieaGameEngine
{

	RTTI_DEFINITIONS(Scope);

	Scope::Scope(std::uint32_t initialSize) :
		mParent(nullptr), mOrderVector(initialSize)
	{
		
	}

	Scope::~Scope()
	{
		Orphan();
		Clear();
	}

	Scope::Scope(const Scope& otherScope) :
		Scope()
	{
		*this = otherScope;
	}

	Scope& Scope::operator=(const Scope& otherScope)
	{
		if (this != &otherScope)
		{
			Clear();

			for (PairType* pair : otherScope.mOrderVector)
			{
				Datum& datum = pair->second;

				if (datum.Type() == Datum::DatumType::Table)
				{
					Datum& newTable = Append(pair->first);
					newTable.SetType(Datum::DatumType::Table);

					for (std::uint32_t i = 0; i < datum.Size(); ++i)
					{
						Scope* newScope = datum.Get<Scope&>(i).Copy();
						newScope->mParent = this;
						newTable.PushBack(*newScope);
					}
				}
				else
				{
					Iterator it = mMap.Insert(*pair);
					mOrderVector.PushBack(&(*it));
				}
			}
		}

		return *this;
	}

	Datum* Scope::Find(const std::string& key)
	{
		return const_cast<Datum*>(const_cast<const Scope*>(this)->Find(key));
	}

	const Datum* Scope::Find(const std::string& key) const
	{
		Datum* foundDatum = nullptr;
		Iterator it = mMap.Find(key);

		if (it != mMap.end())
		{
			foundDatum = &(it->second);
		}

		return foundDatum;
	}

	Datum* Scope::Search(const std::string& key, Scope** containingScope)
	{
		return const_cast<Datum*>(const_cast<const Scope*>(this)->Search(key, containingScope));
	}

	const Datum* Scope::Search(const std::string& key, Scope** containingScope) const
	{
		Datum* datum = const_cast<Datum*>(Find(key));

		if (datum == nullptr)
		{
			if (mParent != nullptr)
			{
				datum = mParent->Search(key, containingScope);
			}
			else
			{
				if (containingScope != nullptr)
				{
					*containingScope = nullptr;
				}
			}
		}
		else if (containingScope != nullptr)
		{
			*containingScope = const_cast<Scope*>(this);
		}

		return datum;
	}

	Datum& Scope::Append(const std::string& key)
	{
		bool inserted = false;

		Iterator it = mMap.Insert(PairType(key, Datum()), inserted);

		if (inserted)
		{
			mOrderVector.PushBack(&(*it));
		}

		return it->second;
	}

	Scope& Scope::AppendScope(const std::string& key)
	{
		Datum& datum = Append(key);

		if (datum.Type() != Datum::DatumType::Unknown && datum.Type() != Datum::DatumType::Table)
		{
			throw std::exception("Datum exists with non-table type");
		}
		
		Scope* appendedScope = new Scope();
		appendedScope->mParent = this;

		datum.PushBack(*appendedScope);

		return *appendedScope;
	}

	void Scope::Adopt(Scope& child, const std::string& key)
	{
		if (this == &child)
		{
			throw std::exception("Cannot Adopt self");
		}

		Datum& datum = Append(key);

		if (datum.Type() != Datum::DatumType::Unknown && datum.Type() != Datum::DatumType::Table)
		{
			throw std::exception("Datum exists with non-table type");
		}

		child.Orphan();

		child.mParent = this;

		datum.PushBack(child);
	}

	Scope* Scope::GetParent()
	{
		return mParent;
	}

	const Scope* Scope::GetParent() const
	{
		return mParent;
	}

	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key);
	}

	const Datum& Scope::operator[](const std::string& key) const
	{
		return mMap[key];
	}

	Datum& Scope::operator[](const std::uint32_t index)
	{
		return const_cast<Datum&>(const_cast<const Scope*>(this)->operator[](index));
	}

	const Datum& Scope::operator[](const std::uint32_t index) const
	{
		return mOrderVector[index]->second;
	}

	bool Scope::operator==(const Scope& otherScope) const
	{
		return this == &otherScope || mMap == otherScope.mMap;
	}

	bool Scope::operator!=(const Scope& otherScope) const
	{
		return !(*this == otherScope);
	}

	bool Scope::Equals(const RTTI* otherScope) const
	{
		if (this == otherScope)
		{
			return true;
		}

		if (otherScope == nullptr)
		{
			return false;
		}

		Scope* scope = otherScope->As<Scope>();

		if (scope == nullptr)
		{
			return false;
		}

		return *this == *scope;
	}

	void Scope::Clear()
	{
		for (PairType* pair : mOrderVector)
		{
			Datum& datum = pair->second;

			if (datum.Type() == Datum::DatumType::Table)
			{
				for (std::uint32_t i = 0; i < datum.Size(); ++i)
				{
					Scope* scope = &datum.Get<Scope&>(i);
					scope->mParent = nullptr;
					delete(scope);
				}
			}
		}

		mMap.Clear();
		mOrderVector.Clear();
	}

	void Scope::Orphan()
	{
		std::string self;
		Orphan(self);
	}

	void Scope::Orphan(std::string& selfKey)
	{
		if (mParent != nullptr)
		{
			for (PairType* pair : mParent->mOrderVector)
			{
				if (pair->second.Remove(*this))
				{
					selfKey = pair->first;

					break;
				}
			}

			mParent = nullptr;
		}
	}

	Scope::Scope(Scope&& previousScope) :
		mParent(previousScope.mParent), mMap(std::move(previousScope.mMap)),
		mOrderVector(std::move(previousScope.mOrderVector))
	{
		Reparent(previousScope);
		previousScope.mParent = nullptr;
	}

	Scope& Scope::operator=(Scope&& previousScope)
	{
		if (&previousScope != this)
		{
			Clear();

			mParent = previousScope.mParent;
			mMap = std::move(previousScope.mMap);
			mOrderVector = std::move(previousScope.mOrderVector);

			Reparent(previousScope);
			previousScope.mParent = nullptr;
		}

		return *this;
	}

	Datum* Scope::FindContainedScope(Scope& scopeToFind, std::uint32_t& index) const
	{
		Datum* foundDatum = nullptr;

		for (PairType* pair : mOrderVector)
		{
			Datum& datum = pair->second;

			if (datum.Type() == Datum::DatumType::Table)
			{
				for (std::uint32_t i = 0; i < datum.Size(); ++i)
				{
					if (datum.Get<Scope&>(i) == scopeToFind)
					{
						index = i;
						foundDatum = &datum;
						break;
					}
				}
			}
		}

		return foundDatum;
	}

	Scope* Scope::Copy() const
	{
		return new Scope(*this);
	}

	void Scope::CopyEntryTo(const std::uint32_t index, Scope& otherScope) const
	{
		otherScope.Append(mOrderVector[index]->first) = mOrderVector[index]->second;
	}

	void Scope::Reparent(Scope& previousScope)
	{
		if (mParent != nullptr)
		{
			std::uint32_t index = mOrderVector.Size();
			Datum* containingDatum = mParent->FindContainedScope(previousScope, index);

			if (containingDatum != nullptr)
			{
				containingDatum->Set(*this, index);
			}
		}

		for (PairType* pair : mOrderVector)
		{
			Datum& datum = pair->second;

			if (datum.Type() == Datum::DatumType::Table)
			{
				for (std::uint32_t i = 0; i < datum.Size(); ++i)
				{
					datum.Get<Scope&>(i).mParent = this;
				}
			}
		}
	}
}