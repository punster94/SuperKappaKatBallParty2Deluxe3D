#include "pch.h"

namespace FieaGameEngine
{
	Datum::Datum() :
		mType(Datum::DatumType::Unknown), mSize(0), mCapacity(0), mExternal(false)
	{
		mData.vp = nullptr;
	}

	Datum::Datum(const Datum& otherDatum) :
		Datum()
	{
		*this = otherDatum;
	}

	Datum& Datum::operator=(const Datum& otherDatum)
	{
		if (this != &otherDatum)
		{
			Clear();

			mType = otherDatum.mType;

			if (!otherDatum.mExternal)
			{
				Reserve(otherDatum.mCapacity);

				mSize = 0;

				for (std::uint32_t i = 0; i < otherDatum.mSize; ++i)
				{
					(this->*CopyElementFunctions[mType])(otherDatum, i);
				}
			}
			else
			{
				mSize = otherDatum.mSize;
				mCapacity = otherDatum.mCapacity;
				mData.vp = otherDatum.mData.vp;
			}

			mExternal = otherDatum.mExternal;
		}

		return *this;
	}

	Datum& Datum::operator=(const std::int32_t& i)
	{
		if (mType == Datum::DatumType::Integer)
		{
			mType = Datum::DatumType::Float;
		}

		if (mSize == 0)
		{
			PushBack(i);
		}
		else
		{
			Set(i);
		}

		return *this;
	}

	Datum& Datum::operator=(const float& f)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Float;
		}

		if (mSize == 0)
		{
			PushBack(f);
		}
		else
		{
			Set(f);
		}

		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& v)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Vec4;
		}

		if (mSize == 0)
		{
			PushBack(v);
		}
		else
		{
			Set(v);
		}

		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& m)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Mat4;
		}

		if (mSize == 0)
		{
			PushBack(m);
		}
		else
		{
			Set(m);
		}

		return *this;
	}

	Datum& Datum::operator=(const std::string& s)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::String;
		}

		if (mSize == 0)
		{
			PushBack(s);
		}
		else
		{
			Set(s);
		}

		return *this;
	}

	Datum& Datum::operator=(RTTI* p)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Pointer;
		}

		if (mSize == 0)
		{
			PushBack(p);
		}
		else
		{
			Set(p);
		}

		return *this;
	}

	Datum& Datum::operator=(Scope& t)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Table;
		}

		if (mSize == 0)
		{
			PushBack(t);
		}
		else
		{
			Set(t);
		}

		return *this;
	}

	Datum::~Datum()
	{
		if (!mExternal)
		{
			Clear();

			free(mData.vp);
		}
	}

	bool Datum::IsExternal() const
	{
		return mExternal;
	}

	Datum::DatumType Datum::Type() const
	{
		return mType;
	}

	void Datum::SetType(const DatumType type)
	{
		if (mType != Datum::DatumType::Unknown)
		{
			throw std::exception("Cannot reset type");
		}

		mType = type;
	}

	std::uint32_t Datum::Size() const
	{
		return mSize;
	}

	std::uint32_t Datum::Capacity() const
	{
		return mCapacity;
	}

	void Datum::SetSize(const std::uint32_t size)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			throw std::exception("Cannot SetSize for Datum of Unknown type");
		}

		if (mExternal)
		{
			throw std::exception("Cannot SetSize for Datum with external storage references");
		}

		if (size > mCapacity)
		{
			Reserve(size);
		}
		else
		{
			for (std::uint32_t i = size; i < mSize; ++i)
			{
				(this->*DestructorFunctions[mType])(i);
			}
		}

		for (std::uint32_t i = mSize; i < size; ++i)
		{
			(this->*PushBackDefaultFunctions[mType])();
		}

		mSize = size;
	}

	void Datum::Clear()
	{
		if (!mExternal)
		{
			for (std::uint32_t i = 0; i < mSize; ++i)
			{
				(this->*DestructorFunctions[mType])(i);
			}

			free(mData.vp);
		}

		mData.vp = nullptr;
		mType = Datum::DatumType::Unknown;
		mSize = 0;
		mCapacity = 0;
		mExternal = false;
	}

	void Datum::SetStorage(std::int32_t* data, const std::uint32_t count)
	{
		if (count == 0)
		{
			throw std::exception("Cannot set storage of size 0");
		}

		Clear();

		mData.i = data;
		mType = Datum::DatumType::Integer;
		mSize = count;
		mCapacity = count;
		mExternal = true;
	}

	void Datum::SetStorage(float* data, const std::uint32_t count)
	{
		if (count == 0)
		{
			throw std::exception("Cannot set storage of size 0");
		}

		Clear();

		mData.f = data;
		mType = Datum::DatumType::Float;
		mSize = count;
		mCapacity = count;
		mExternal = true;
	}

	void Datum::SetStorage(glm::vec4* data, const std::uint32_t count)
	{
		if (count == 0)
		{
			throw std::exception("Cannot set storage of size 0");
		}

		Clear();

		mData.v = data;
		mType = Datum::DatumType::Vec4;
		mSize = count;
		mCapacity = count;
		mExternal = true;
	}

	void Datum::SetStorage(glm::mat4* data, const std::uint32_t count)
	{
		if (count == 0)
		{
			throw std::exception("Cannot set storage of size 0");
		}

		Clear();

		mData.m = data;
		mType = Datum::DatumType::Mat4;
		mSize = count;
		mCapacity = count;
		mExternal = true;
	}

	void Datum::SetStorage(std::string* data, const std::uint32_t count)
	{
		if (count == 0)
		{
			throw std::exception("Cannot set storage of size 0");
		}

		Clear();

		mData.s = data;
		mType = Datum::DatumType::String;
		mSize = count;
		mCapacity = count;
		mExternal = true;
	}

	void Datum::SetStorage(RTTI** data, const std::uint32_t count)
	{
		if (count == 0)
		{
			throw std::exception("Cannot set storage of size 0");
		}

		Clear();

		mData.p = data;
		mType = Datum::DatumType::Pointer;
		mSize = count;
		mCapacity = count;
		mExternal = true;
	}

	bool Datum::operator==(const Datum& otherDatum) const
	{
		bool returnValue = (mType == otherDatum.mType && mSize == otherDatum.mSize);

		if (returnValue)
		{
			for (std::uint32_t i = 0; i < mSize && returnValue; ++i)
			{
				if (!((this->*IndexEqualityFunctions[mType])(otherDatum, i)))
				{
					returnValue = false;
					break;
				}
			}
		}

		return returnValue;
	}

	bool Datum::operator==(const std::int32_t i) const
	{
		return mType == Datum::DatumType::Integer &&
			mSize == 1 &&
			mData.i[0] == i;
	}

	bool Datum::operator==(const float f) const
	{
		return mType == Datum::DatumType::Float &&
			mSize == 1 &&
			mData.f[0] == f;
	}

	bool Datum::operator==(const glm::vec4 v) const
	{
		return mType == Datum::DatumType::Vec4 &&
			mSize == 1 &&
			mData.v[0] == v;
	}

	bool Datum::operator==(const glm::mat4 m) const
	{
		return mType == Datum::DatumType::Mat4 &&
			mSize == 1 &&
			mData.m[0] == m;
	}

	bool Datum::operator==(const std::string s) const
	{
		return mType == Datum::DatumType::String &&
			mSize == 1 &&
			mData.s[0] == s;
	}

	bool Datum::operator==(const RTTI* p) const
	{
		return mType == Datum::DatumType::Pointer &&
			mSize == 1 &&
			mData.p[0]->Equals(p);
	}

	bool Datum::operator==(const Scope& t) const
	{
		return mType == Datum::DatumType::Table &&
			mSize == 1 &&
			mData.t[0] != nullptr &&
			mData.t[0]->Equals(&t);
	}

	bool Datum::operator!=(const Datum& otherDatum) const
	{
		return !(*this == otherDatum);
	}

	bool Datum::operator!=(const std::int32_t i) const
	{
		return !(*this == i);
	}

	bool Datum::operator!=(const float f) const
	{
		return !(*this == f);
	}

	bool Datum::operator!=(const glm::vec4 v) const
	{
		return !(*this == v);
	}

	bool Datum::operator!=(const glm::mat4 m) const
	{
		return !(*this == m);
	}

	bool Datum::operator!=(const std::string s) const
	{
		return !(*this == s);
	}

	bool Datum::operator!=(const RTTI* p) const
	{
		return !(*this == p);
	}

	bool Datum::operator!=(const Scope& t) const
	{
		return !(*this == t);
	}

	void Datum::Set(const std::int32_t& i, const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Cannot set value out of range");
		}

		if (mType != Datum::DatumType::Integer)
		{
			throw std::exception("Invalid type");
		}

		mData.i[index] = i;
	}

	void Datum::Set(const float& f, const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Cannot set value out of range");
		}

		if (mType != Datum::DatumType::Float)
		{
			throw std::exception("Invalid type");
		}

		mData.f[index] = f;
	}

	void Datum::Set(const glm::vec4& v, const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Cannot set value out of range");
		}

		if (mType != Datum::DatumType::Vec4)
		{
			throw std::exception("Invalid type");
		}

		mData.v[index] = v;
	}

	void Datum::Set(const glm::mat4& m, const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Cannot set value out of range");
		}

		if (mType != Datum::DatumType::Mat4)
		{
			throw std::exception("Invalid type");
		}

		mData.m[index] = m;
	}

	void Datum::Set(const std::string& s, const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Cannot set value out of range");
		}

		if (mType != Datum::DatumType::String)
		{
			throw std::exception("Invalid type");
		}

		mData.s[index] = s;
	}

	void Datum::Set(RTTI* p, const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Cannot set value out of range");
		}

		if (mType != Datum::DatumType::Pointer)
		{
			throw std::exception("Invalid type");
		}

		mData.p[index] = p;
	}

	void Datum::Set(Scope& t, const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Cannot set value out of range");
		}

		if (mType != Datum::DatumType::Table)
		{
			throw std::exception("Invalid type");
		}

		mData.t[index] = &t;
	}

	void Datum::PushBack(const std::int32_t& i)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Integer;
		}

		if (mType != Datum::DatumType::Integer)
		{
			throw std::exception("Invalid type");
		}

		if (mSize == mCapacity)
		{
			Reserve(mSize + 1);
		}

		new (mData.i + mSize++) std::int32_t(i);
	}

	void Datum::PushBack(const float& f)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Float;
		}

		if (mType != Datum::DatumType::Float)
		{
			throw std::exception("Invalid type");
		}

		if (mSize == mCapacity)
		{
			Reserve(mSize + 1);
		}

		new (mData.f + mSize++) float(f);
	}

	void Datum::PushBack(const glm::vec4& v)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Vec4;
		}

		if (mType != Datum::DatumType::Vec4)
		{
			throw std::exception("Invalid type");
		}

		if (mSize == mCapacity)
		{
			Reserve(mSize + 1);
		}

		new (mData.v + mSize++) glm::vec4(v);
	}

	void Datum::PushBack(const glm::mat4& m)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Mat4;
		}

		if (mType != Datum::DatumType::Mat4)
		{
			throw std::exception("Invalid type");
		}

		if (mSize == mCapacity)
		{
			Reserve(mSize + 1);
		}

		new (mData.m + mSize++) glm::mat4(m);
	}

	void Datum::PushBack(const std::string& s)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::String;
		}

		if (mType != Datum::DatumType::String)
		{
			throw std::exception("Invalid type");
		}

		if (mSize == mCapacity)
		{
			Reserve(mSize + 1);
		}

		new (mData.s + mSize++) std::string(s);
	}

	void Datum::PushBack(RTTI* p)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Pointer;
		}

		if (mType != Datum::DatumType::Pointer)
		{
			throw std::exception("Invalid type");
		}

		if (mSize == mCapacity)
		{
			Reserve(mSize + 1);
		}

		new (mData.p + mSize++) RTTI*(p);
	}

	void Datum::PushBack(Scope& t)
	{
		if (mType == Datum::DatumType::Unknown)
		{
			mType = Datum::DatumType::Table;
		}

		if (mType != Datum::DatumType::Table)
		{
			throw std::exception("Invalid type");
		}

		if (mSize == mCapacity)
		{
			Reserve(mSize + 1);
		}

		new (mData.t + mSize++) Scope*(&t);
	}

	template<>
	std::int32_t& Datum::Get<std::int32_t&>(const std::uint32_t index)
	{
		return const_cast<std::int32_t&>(const_cast<const Datum*> (this)->Get<const std::int32_t&>(index));
	}

	template<>
	float& Datum::Get<float&>(const std::uint32_t index)
	{
		return const_cast<float&>(const_cast<const Datum*> (this)->Get<const float&>(index));
	}

	template<>
	glm::vec4& Datum::Get<glm::vec4&>(const std::uint32_t index)
	{
		return const_cast<glm::vec4&>(const_cast<const Datum*> (this)->Get<const glm::vec4&>(index));
	}

	template<>
	glm::mat4& Datum::Get<glm::mat4&>(const std::uint32_t index)
	{
		return const_cast<glm::mat4&>(const_cast<const Datum*> (this)->Get<const glm::mat4&>(index));
	}

	template<>
	std::string& Datum::Get<std::string&>(const std::uint32_t index)
	{
		return const_cast<std::string&>(const_cast<const Datum*> (this)->Get<const std::string&>(index));
	}

	template<>
	RTTI*& Datum::Get<RTTI*&>(const std::uint32_t index)
	{
		return const_cast<RTTI*&>(const_cast<const Datum*> (this)->Get<const RTTI*&>(index));
	}

	template<>
	Scope& Datum::Get<Scope&>(const std::uint32_t index)
	{
		return const_cast<Scope&>(const_cast<const Datum*> (this)->Get<const Scope&>(index));
	}

	template<>
	const std::int32_t& Datum::Get<const std::int32_t&>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range");
		}

		if (mType != Datum::DatumType::Integer)
		{
			throw std::exception("Invalid type");
		}

		return mData.i[index];
	}

	template<>
	const float& Datum::Get<const float&>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range");
		}

		if (mType != Datum::DatumType::Float)
		{
			throw std::exception("Invalid type");
		}

		return mData.f[index];
	}

	template<>
	const glm::vec4& Datum::Get<const glm::vec4&>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range");
		}

		if (mType != Datum::DatumType::Vec4)
		{
			throw std::exception("Invalid type");
		}

		return mData.v[index];
	}

	template<>
	const glm::mat4& Datum::Get<const glm::mat4&>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range");
		}

		if (mType != Datum::DatumType::Mat4)
		{
			throw std::exception("Invalid type");
		}

		return mData.m[index];
	}

	template<>
	const std::string& Datum::Get<const std::string&>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range");
		}

		if (mType != Datum::DatumType::String)
		{
			throw std::exception("Invalid type");
		}

		return mData.s[index];
	}

	template<>
	const RTTI*& Datum::Get<const RTTI*&>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range");
		}

		if (mType != Datum::DatumType::Pointer)
		{
			throw std::exception("Invalid type");
		}

		return const_cast<const RTTI*&>(mData.p[index]);
	}

	template<>
	const Scope& Datum::Get<const Scope&>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range");
		}

		if (mType != Datum::DatumType::Table)
		{
			throw std::exception("Invalid type");
		}

		return const_cast<const Scope&>(*mData.t[index]);
	}

	void Datum::SetFromString(const std::string& s, const std::uint32_t index)
	{
		if (mType == DatumType::Unknown || mType == DatumType::Pointer || mType == DatumType::Table)
		{
			throw std::exception("Invalid type");
		}

		(this->*SetFromStringFunctions[mType])(s, index);
	}

	void Datum::PushBackFromString(const std::string& s)
	{
		if (mType == DatumType::Unknown || mType == DatumType::Pointer || mType == DatumType::Table)
		{
			throw std::exception("Invalid type");
		}

		(this->*PushBackFromStringFunctions[mType])(s);
	}

	std::string Datum::ToString(const std::uint32_t index) const
	{
		if (mType == Datum::DatumType::Unknown)
		{
			throw std::exception("Invalid type");
		}

		return (this->*ToStringFunctions[mType])(index);
	}

	void Datum::Reserve(const std::uint32_t size)
	{
		if (mExternal)
		{
			throw std::exception("Cannot resize Datum with external storage references");
		}

		if (size > mCapacity)
		{
			mCapacity = size;

			if (mType == Datum::DatumType::Unknown)
			{
				throw std::exception("Cannot resize Datum of Unknown type");
			}

			mData.vp = realloc(mData.vp, mCapacity * DatumTypeSizes[mType]);
		}
	}

	bool Datum::Remove(Scope& child)
	{
		bool removed = false;

		if (mType == DatumType::Table)
		{
			std::uint32_t i;

			for (i = 0; i < mSize; ++i)
			{
				if (&Get<Scope&>(i) == &child)
				{
					break;
				}
			}

			if (i != mSize)
			{
				while (++i < mSize)
				{
					Set(Get<Scope&>(i), i - 1);
				}

				mData.t[i - 1] = nullptr;

				--mSize;

				removed = true;
			}
		}

		return removed;
	}

	Datum::Datum(Datum&& previousDatum) :
		mData(previousDatum.mData), mType(previousDatum.mType), mSize(previousDatum.mSize),
		mCapacity(previousDatum.mCapacity), mExternal(previousDatum.mExternal)
	{
		previousDatum.mData.vp = nullptr;
		previousDatum.mType = DatumType::Unknown;
		previousDatum.mSize = 0;
		previousDatum.mCapacity = 0;
		previousDatum.mExternal = false;
	}

	Datum& Datum::operator=(Datum&& previousDatum)
	{
		if (this != &previousDatum)
		{
			Clear();

			mData.vp = previousDatum.mData.vp;
			mType = previousDatum.mType;
			mSize = previousDatum.mSize;
			mCapacity = previousDatum.mCapacity;
			mExternal = previousDatum.mExternal;

			previousDatum.mData.vp = nullptr;
			previousDatum.mType = DatumType::Unknown;
			previousDatum.mSize = 0;
			previousDatum.mCapacity = 0;
			previousDatum.mExternal = false;
		}

		return *this;
	}

#pragma region
	void Datum::CopyInt(const Datum& otherDatum, const std::uint32_t index)
	{
		PushBack(otherDatum.mData.i[index]);
	}

	void Datum::CopyFloat(const Datum& otherDatum, const std::uint32_t index)
	{
		PushBack(otherDatum.mData.f[index]);
	}

	void Datum::CopyVec4(const Datum& otherDatum, const std::uint32_t index)
	{
		PushBack(otherDatum.mData.v[index]);
	}

	void Datum::CopyMat4(const Datum& otherDatum, const std::uint32_t index)
	{
		PushBack(otherDatum.mData.m[index]);
	}

	void Datum::CopyString(const Datum& otherDatum, const std::uint32_t index)
	{
		PushBack(otherDatum.mData.s[index]);
	}

	void Datum::CopyRTTI(const Datum& otherDatum, const std::uint32_t index)
	{
		PushBack(otherDatum.mData.p[index]);
	}

	void Datum::CopyScope(const Datum& otherDatum, const std::uint32_t index)
	{
		PushBack(*otherDatum.mData.t[index]);
	}
#pragma endregion CopyElement Methods

#pragma region
	void Datum::PushBackDefaultInt()
	{
		PushBack(std::int32_t());
	}

	void Datum::PushBackDefaultFloat()
	{
		PushBack(float());
	}

	void Datum::PushBackDefaultVec4()
	{
		PushBack(glm::vec4());
	}

	void Datum::PushBackDefaultMat4()
	{
		PushBack(glm::mat4());
	}

	void Datum::PushBackDefaultString()
	{
		PushBack(std::string());
	}

	void Datum::PushBackDefaultRTTI()
	{
		PushBack(static_cast<RTTI*>(nullptr));
	}

	void Datum::PushBackDefaultScope()
	{
		mData.t[mSize] = nullptr;
		++mSize;
	}
#pragma endregion PushBackDefault Methods

#pragma region
	void Datum::PrimitiveDestructor(const std::uint32_t index)
	{
		UNREFERENCED_PARAMETER(index);
	}

	void Datum::StringDestructor(const std::uint32_t index)
	{
		mData.s[index].std::string::~string();
	}
#pragma endregion Destructor Methods

#pragma region
	bool Datum::IntIndexEquality(const Datum& otherDatum, const std::uint32_t index) const
	{
		return mData.i[index] == otherDatum.mData.i[index];
	}

	bool Datum::FloatIndexEquality(const Datum& otherDatum, const std::uint32_t index) const
	{
		return mData.f[index] == otherDatum.mData.f[index];
	}

	bool Datum::Vec4IndexEquality(const Datum& otherDatum, const std::uint32_t index) const
	{
		return mData.v[index] == otherDatum.mData.v[index];
	}

	bool Datum::Mat4IndexEquality(const Datum& otherDatum, const std::uint32_t index) const
	{
		return mData.m[index] == otherDatum.mData.m[index];
	}

	bool Datum::StringIndexEquality(const Datum& otherDatum, const std::uint32_t index) const
	{
		return mData.s[index] == otherDatum.mData.s[index];
	}

	bool Datum::RTTIIndexEquality(const Datum& otherDatum, const std::uint32_t index) const
	{
		if (mData.p[index] == nullptr)
		{
			return otherDatum.mData.p[index] == nullptr;
		}

		return mData.p[index]->Equals(otherDatum.mData.p[index]);
	}

	bool Datum::ScopeIndexEquality(const Datum& otherDatum, const std::uint32_t index) const
	{
		return mData.t[index]->Equals(otherDatum.mData.t[index]);
	}
#pragma endregion IndexEquality Methods

#pragma region
	void Datum::IntFromString(const std::string& s, const std::uint32_t index)
	{
		const char* buffer = s.c_str();
		std::int32_t iValue;
		std::uint32_t successes = sscanf_s(buffer, "%d", &iValue);

		if (successes != 1)
		{
			throw std::exception("Invalid format");
		}

		Set(iValue, index);
	}

	void Datum::FloatFromString(const std::string& s, const std::uint32_t index)
	{
		const char* buffer = s.c_str();
		float fValue;
		std::uint32_t successes = sscanf_s(buffer, "%f", &fValue);

		if (successes != 1)
		{
			throw std::exception("Invalid format");
		}

		Set(fValue, index);
	}

	void Datum::Vec4FromString(const std::string& s, const std::uint32_t index)
	{
		const char* buffer = s.c_str();
		glm::vec4 vValue;
		std::uint32_t successes = sscanf_s(buffer, "%f,%f,%f,%f,", &vValue[0], &vValue[1], &vValue[2], &vValue[3]);

		if (successes != 4)
		{
			throw std::exception("Invalid format");
		}

		Set(vValue, index);
	}

	void Datum::Mat4FromString(const std::string& s, const std::uint32_t index)
	{
		const char* buffer = s.c_str();
		glm::mat4 mValue;
		std::uint32_t successes = sscanf_s(buffer, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,",
			&mValue[0][0], &mValue[0][1], &mValue[0][2], &mValue[0][3],
			&mValue[1][0], &mValue[1][1], &mValue[1][2], &mValue[1][3],
			&mValue[2][0], &mValue[2][1], &mValue[2][2], &mValue[2][3],
			&mValue[3][0], &mValue[3][1], &mValue[3][2], &mValue[3][3]);

		if (successes != 16)
		{
			throw std::exception("Invalid format");
		}

		Set(mValue, index);
	}

	void Datum::StringFromString(const std::string& s, const std::uint32_t index)
	{
		Set(s, index);
	}
#pragma endregion SetFromString Methods

#pragma region
	void Datum::PushIntFromString(const std::string& s)
	{
		const char* buffer = s.c_str();
		std::int32_t iValue;
		std::uint32_t successes = sscanf_s(buffer, "%d", &iValue);

		if (successes != 1)
		{
			throw std::exception("Invalid format");
		}

		PushBack(iValue);
	}

	void Datum::PushFloatFromString(const std::string& s)
	{
		const char* buffer = s.c_str();
		float fValue;
		std::uint32_t successes = sscanf_s(buffer, "%f", &fValue);

		if (successes != 1)
		{
			throw std::exception("Invalid format");
		}

		PushBack(fValue);
	}

	void Datum::PushVec4FromString(const std::string& s)
	{
		const char* buffer = s.c_str();
		glm::vec4 vValue;
		std::uint32_t successes = sscanf_s(buffer, "%f,%f,%f,%f,", &vValue[0], &vValue[1], &vValue[2], &vValue[3]);

		if (successes != 4)
		{
			throw std::exception("Invalid format");
		}

		PushBack(vValue);
	}

	void Datum::PushMat4FromString(const std::string& s)
	{
		const char* buffer = s.c_str();
		glm::mat4 mValue;
		std::uint32_t successes = sscanf_s(buffer, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,",
			&mValue[0][0], &mValue[0][1], &mValue[0][2], &mValue[0][3],
			&mValue[1][0], &mValue[1][1], &mValue[1][2], &mValue[1][3],
			&mValue[2][0], &mValue[2][1], &mValue[2][2], &mValue[2][3],
			&mValue[3][0], &mValue[3][1], &mValue[3][2], &mValue[3][3]);

		if (successes != 16)
		{
			throw std::exception("Invalid format");
		}

		PushBack(mValue);
	}

	void Datum::PushStringFromString(const std::string& s)
	{
		PushBack(s);
	}
#pragma endregion PushBackFromString Methods

#pragma region
	std::string Datum::IntToString(const std::uint32_t index) const
	{
		char buffer[20];
		sprintf_s(buffer, "%d", Get<const std::int32_t&>(index));

		return buffer;
	}

	std::string Datum::FloatToString(const std::uint32_t index) const
	{
		char buffer[20];
		sprintf_s(buffer, "%f", Get<const float&>(index));

		return buffer;
	}

	std::string Datum::Vec4ToString(const std::uint32_t index) const
	{
		char buffer[20];
		std::string returnValue = "";
		glm::vec4 vector = Get<const glm::vec4&>(index);

		for (std::uint32_t i = 0; i < 4; ++i)
		{
			sprintf_s(buffer, "%f,", vector[i]);
			returnValue += buffer;
		}

		return returnValue;
	}

	std::string Datum::Mat4ToString(const std::uint32_t index) const
	{
		char buffer[20];
		std::string returnValue = "";
		glm::mat4 matrix = Get<const glm::mat4&>(index);

		for (std::uint32_t i = 0; i < 4; ++i)
		{
			for (std::uint32_t j = 0; j < 4; ++j)
			{
				sprintf_s(buffer, "%f,", matrix[i][j]);
				returnValue += buffer;
			}
		}

		return returnValue;
	}

	std::string Datum::StringToString(const std::uint32_t index) const
	{
		return Get<const std::string&>(index);
	}

	std::string Datum::RTTIToString(const std::uint32_t index) const
	{
		return Get<const RTTI*&>(index)->ToString();
	}

	std::string Datum::ScopeToString(const std::uint32_t index) const
	{
		return Get<const Scope&>(index).ToString();
	}

#pragma endregion ToString Methods

#pragma region
	const HashMap<Datum::DatumType, size_t> Datum::DatumTypeSizes =
	{
		{ Datum::DatumType::Integer, sizeof(std::int32_t) },
		{ Datum::DatumType::Float, sizeof(float) },
		{ Datum::DatumType::Vec4, sizeof(glm::vec4) },
		{ Datum::DatumType::Mat4, sizeof(glm::mat4) },
		{ Datum::DatumType::String, sizeof(std::string) },
		{ Datum::DatumType::Pointer, sizeof(RTTI*) },
		{ Datum::DatumType::Table, sizeof(Scope*) },
	};

	const HashMap<Datum::DatumType, Datum::ToStringFunction> Datum::ToStringFunctions =
	{
		{ Datum::DatumType::Integer, &Datum::IntToString },
		{ Datum::DatumType::Float, &Datum::FloatToString },
		{ Datum::DatumType::Vec4, &Datum::Vec4ToString },
		{ Datum::DatumType::Mat4, &Datum::Mat4ToString },
		{ Datum::DatumType::String, &Datum::StringToString },
		{ Datum::DatumType::Pointer, &Datum::RTTIToString },
		{ Datum::DatumType::Table, &Datum::ScopeToString },
	};

	const HashMap<Datum::DatumType, Datum::PushBackFromStringFunction> Datum::PushBackFromStringFunctions =
	{
		{ Datum::DatumType::Integer, &Datum::PushIntFromString },
		{ Datum::DatumType::Float, &Datum::PushFloatFromString },
		{ Datum::DatumType::Vec4, &Datum::PushVec4FromString },
		{ Datum::DatumType::Mat4, &Datum::PushMat4FromString },
		{ Datum::DatumType::String, &Datum::PushStringFromString },
	};

	const HashMap<Datum::DatumType, Datum::SetFromStringFunction> Datum::SetFromStringFunctions =
	{
		{ Datum::DatumType::Integer, &Datum::IntFromString },
		{ Datum::DatumType::Float, &Datum::FloatFromString },
		{ Datum::DatumType::Vec4, &Datum::Vec4FromString },
		{ Datum::DatumType::Mat4, &Datum::Mat4FromString },
		{ Datum::DatumType::String, &Datum::StringFromString },
	};

	const HashMap<Datum::DatumType, Datum::IndexEqualityFunction> Datum::IndexEqualityFunctions =
	{
		{ Datum::DatumType::Integer, &Datum::IntIndexEquality },
		{ Datum::DatumType::Float, &Datum::FloatIndexEquality },
		{ Datum::DatumType::Vec4, &Datum::Vec4IndexEquality },
		{ Datum::DatumType::Mat4, &Datum::Mat4IndexEquality },
		{ Datum::DatumType::String, &Datum::StringIndexEquality },
		{ Datum::DatumType::Pointer, &Datum::RTTIIndexEquality },
		{ Datum::DatumType::Table, &Datum::ScopeIndexEquality },
	};

	const HashMap<Datum::DatumType, Datum::DestructorFunction> Datum::DestructorFunctions =
	{
		{ Datum::DatumType::Integer, &Datum::PrimitiveDestructor },
		{ Datum::DatumType::Float, &Datum::PrimitiveDestructor },
		{ Datum::DatumType::Vec4, &Datum::PrimitiveDestructor },
		{ Datum::DatumType::Mat4, &Datum::PrimitiveDestructor },
		{ Datum::DatumType::String, &Datum::StringDestructor },
		{ Datum::DatumType::Pointer, &Datum::PrimitiveDestructor },
		{ Datum::DatumType::Table, &Datum::PrimitiveDestructor },
	};

	const HashMap<Datum::DatumType, Datum::PushBackDefaultFunction> Datum::PushBackDefaultFunctions =
	{
		{ Datum::DatumType::Integer, &Datum::PushBackDefaultInt },
		{ Datum::DatumType::Float, &Datum::PushBackDefaultFloat },
		{ Datum::DatumType::Vec4, &Datum::PushBackDefaultVec4 },
		{ Datum::DatumType::Mat4, &Datum::PushBackDefaultMat4 },
		{ Datum::DatumType::String, &Datum::PushBackDefaultString },
		{ Datum::DatumType::Pointer, &Datum::PushBackDefaultRTTI },
		{ Datum::DatumType::Table, &Datum::PushBackDefaultScope },
	};

	const HashMap<Datum::DatumType, Datum::CopyElementFunction> Datum::CopyElementFunctions =
	{
		{ Datum::DatumType::Integer, &Datum::CopyInt },
		{ Datum::DatumType::Float, &Datum::CopyFloat },
		{ Datum::DatumType::Vec4, &Datum::CopyVec4 },
		{ Datum::DatumType::Mat4, &Datum::CopyMat4 },
		{ Datum::DatumType::String, &Datum::CopyString },
		{ Datum::DatumType::Pointer, &Datum::CopyRTTI },
		{ Datum::DatumType::Table, &Datum::CopyScope },
	};
#pragma endregion Function Pointer Maps
}
