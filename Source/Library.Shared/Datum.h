#pragma once

#include <cstdint>
#include <string>
#include "RTTI.h"
#include "glm\glm.hpp"
#include "HashMap.h"

namespace FieaGameEngine
{

	/** Scope class
	 *	Forward declaration of the Scope class for usage in the Datum class.
	 */
	class Scope;

	/** Datum class
	 *	A type-agnostic container for collections of data.
	 */
	class Datum
	{

	public:

		/** DatumType enumeration for Datum
		 *	An enumeration that represents the types of data that can be held in a Datum instance.
		 */
		enum class DatumType
		{
			Unknown,
			Integer,
			Float,
			Vec4,
			Mat4,
			String,
			Pointer,
			Table
		};

	private:

		/** DatumValues union
		 *	A union of pointers used to prevent unnecessary memory allocation.
		 *	Depending on the Datum's type, a different pointer can be used for storage.
		 */
		union DatumValues
		{
			void* vp;
			std::int32_t* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			std::string* s;
			RTTI** p;
			Scope** t;
		};

	public:

		/** Datum Default Constructor
		 *	Sets the new Datum's type to Unknown.
		 */
		Datum();

		/** Datum Copy Constructor
		 *	Copies the given constant Datum reference into this datum.
		 *	@param otherDatum the constant Datum reference to copy from.
		 */
		Datum(const Datum& otherDatum);

		/** Datum Assignment Operator
		 *	Copies the given constant Datum reference into this datum.
		 *	Will clear the contents of this Datum independent of type before copying.
		 *	@param otherDatum the constant Datum reference to copy from.
		 *	@return a Datum reference to this Datum.
		 */
		Datum& operator=(const Datum& otherDatum);

		/** Datum Integer Assignment Operator
		 *	Copies the given constant integer reference into this datum's first index.
		 *	@exception will fail if this Datum is not of type Datum::DatumType::Integer.
		 *	@param i the constant integer reference to copy from.
		 *	@return a Datum reference to this Datum.
		 */
		Datum& operator=(const std::int32_t& i);

		/** Datum Float Assignment Operator
		 *	Copies the given constant float reference into this datum's first index.
		 *	@exception will fail if this Datum is not of type Datum::DatumType::Float.
		 *	@param f the constant float reference to copy from.
		 *	@return a Datum reference to this Datum.
		 */
		Datum& operator=(const float& f);

		/** Datum Vector Assignment Operator
		 *	Copies the given constant glm::vec4 reference into this datum's first index.
		 *	@exception will fail if this Datum is not of type Datum::DatumType::Vec4.
		 *	@param v the constant glm::vec4 reference to copy from.
		 *	@return a Datum reference to this Datum.
		 */
		Datum& operator=(const glm::vec4& v);

		/** Datum Matrix Assignment Operator
		 *	Copies the given constant glm::mat4 reference into this datum's first index.
		 *	@exception will fail if this Datum is not of type Datum::DatumType::Mat4.
		 *	@param m the constant glm::mat4 reference to copy from.
		 *	@return a Datum reference to this Datum.
		 */
		Datum& operator=(const glm::mat4& m);

		/** Datum Integer Assignment Operator
		 *	Copies the given constant string reference into this datum's first index.
		 *	@exception will fail if this Datum is not of type Datum::DatumType::String.
		 *	@param s the constant string reference to copy from.
		 *	@return a Datum reference to this Datum.
		 */
		Datum& operator=(const std::string& s);

		/** Datum Pointer Assignment Operator
		 *	Copies the given RTTI pointer into this datum's first index.
		 *	@exception will fail if this Datum is not of type Datum::DatumType::Pointer.
		 *	@param p the constant RTTI pointer to copy from.
		 *	@return a Datum reference to this Datum.
		 */
		Datum& operator=(RTTI* p);

		/** Datum Table Assignment Operator
		 *	Copies the given RTTI pointer into this datum's first index.
		 *	@exception will fail if this Datum is not of type Datum::DatumType::Pointer.
		 *	@param t the Scope reference to copy from.
		 *	@return a Datum reference to this Datum.
		 */
		Datum& operator=(Scope& t);

		/** Datum Destructor
		 *	Clears and frees the data in the Datum if storage was not externally set.
		 */
		~Datum();

		/** Datum IsExternal
		 *	Returns whether this Datum stores a reference to external data.
		 *	@return a boolean representing whether this Datum stores a reference to external data.
		 */
		bool IsExternal() const;

		/** Datum Type
		 *	Returns the type of this Datum.
		 *	@return the Datum::DatumType that this Datum currently contains.
		 */
		DatumType Type() const;

		/** Datum SetType
		 *	Sets the current type of this Datum to the given Datum::DatumType.
		 *	@exception will fail if the Datum's type is currently set.
		 *	@param type the Datum::DatumType to set this Datum's type to.
		 */
		void SetType(const DatumType type);

		/** Datum Size
		 *	Returns the current size of this Datum.
		 *	@return an unsigned integer representing the size of the Datum.
		 */
		std::uint32_t Size() const;

		/** Datum Capacity
		 *	Returns the current capacity of this Datum.
		 *	@return an unsigned integer representing the capacity of the Datum.
		 */
		std::uint32_t Capacity() const;

		/** Datum SetSize
		 *	Resets the size of the Datum to the specified unsigned integer.
		 *	Will default construct elements if size increase.
		 *	Will destruct trailing elements if the size decreases.
		 *	@exception will fail if the Datum's type is not set.
		 *	@exception will fail if the Datum was set to contain external memory.
		 *	@param size the unsigned integer to set the size and capacity of the Datum to.
		 */
		void SetSize(const std::uint32_t size);

		/** Datum Clear
		 *	Clears the contents of the Datum.
		 *	Sets the Datum's type to Datum::DatumType::Unknown.
		 */
		void Clear();

		/** Datum Integer SetStorage
		 *	Stores the given set of elements in the Datum.
		 *	Clears the contents of the Datum and sets its type to Datum::DatumType::Integer.
		 *	@exception will fail if the passed count is zero.
		 *	@param data an integer pointer representing the external elements to store.
		 *	@param count a constant unsigned integer representing the number of external elements.
		 */
		void SetStorage(std::int32_t* data, const std::uint32_t count);

		/** Datum Float SetStorage
		 *	Stores the given set of elements in the Datum.
		 *	Clears the contents of the Datum and sets its type to Datum::DatumType::Float.
		 *	@exception will fail if the passed count is zero.
		 *	@param data a float pointer representing the external elements to store.
		 *	@param count a constant unsigned integer representing the number of external elements.
		 */
		void SetStorage(float* data, const std::uint32_t count);

		/** Datum Vector SetStorage
		 *	Stores the given set of elements in the Datum.
		 *	Clears the contents of the Datum and sets its type to Datum::DatumType::Vec4.
		 *	@exception will fail if the passed count is zero.
		 *	@param data a glm::vec4 pointer representing the external elements to store.
		 *	@param count a constant unsigned integer representing the number of external elements.
		 */
		void SetStorage(glm::vec4* data, const std::uint32_t count);

		/** Datum Matrix SetStorage
		 *	Stores the given set of elements in the Datum.
		 *	Clears the contents of the Datum and sets its type to Datum::DatumType::Mat4.
		 *	@exception will fail if the passed count is zero.
		 *	@param data a glm::mat4 pointer representing the external elements to store.
		 *	@param count a constant unsigned integer representing the number of external elements.
		 */
		void SetStorage(glm::mat4* data, const std::uint32_t count);

		/** Datum String SetStorage
		 *	Stores the given set of elements in the Datum.
		 *	Clears the contents of the Datum and sets its type to Datum::DatumType::String.
		 *	@exception will fail if the passed count is zero.
		 *	@param data a string pointer representing the external elements to store.
		 *	@param count a constant unsigned integer representing the number of external elements.
		 */
		void SetStorage(std::string* data, const std::uint32_t count);

		/** Datum Pointer SetStorage
		 *	Stores the given set of elements in the Datum.
		 *	Clears the contents of the Datum and sets its type to Datum::DatumType::Pointer.
		 *	@exception will fail if the passed count is zero.
		 *	@param data an RTTI* pointer representing the external elements to store.
		 *	@param count a constant unsigned integer representing the number of external elements.
		 */
		void SetStorage(RTTI** data, const std::uint32_t count);

		/** Datum Equality Operator
		 *	Returns whether the given constant Datum reference is equivalent to this Datum.
		 *	@param otherDatum a constant reference to the Datum to compare with.
		 *	@return a boolean representing whether the given Datum is equivalent to this Datum.
		 */
		bool operator==(const Datum& otherDatum) const;

		/** Datum Integer Equality Operator
		 *	Returns whether the given constant integer reference is equivalent to this Datum.
		 *	Will return false if this Datum contains more than one element.
		 *	@param i a constant reference to the integer to compare with.
		 *	@return a boolean representing whether the given integer is equivalent to this Datum.
		 */
		bool operator==(const std::int32_t i) const;

		/** Datum Float Equality Operator
		 *	Returns whether the given constant float reference is equivalent to this Datum.
		 *	Will return false if this Datum contains more than one element.
		 *	@param f a constant reference to the float to compare with.
		 *	@return a boolean representing whether the given float is equivalent to this Datum.
		 */
		bool operator==(const float f) const;

		/** Datum Vector Equality Operator
		 *	Returns whether the given constant glm::vec4 reference is equivalent to this Datum.
		 *	Will return false if this Datum contains more than one element.
		 *	@param v a constant reference to the glm::vec4 to compare with.
		 *	@return a boolean representing whether the given glm::vec4 is equivalent to this Datum.
		 */
		bool operator==(const glm::vec4 v) const;

		/** Datum Matrix Equality Operator
		 *	Returns whether the given constant glm::mat4 reference is equivalent to this Datum.
		 *	Will return false if this Datum contains more than one element.
		 *	@param m a constant reference to the glm::mat4 to compare with.
		 *	@return a boolean representing whether the given glm::mat4 is equivalent to this Datum.
		 */
		bool operator==(const glm::mat4 m) const;

		/** Datum String Equality Operator
		 *	Returns whether the given constant string reference is equivalent to this Datum.
		 *	Will return false if this Datum contains more than one element.
		 *	@param s a constant reference to the string to compare with.
		 *	@return a boolean representing whether the given string is equivalent to this Datum.
		 */
		bool operator==(const std::string s) const;

		/** Datum Pointer Equality Operator
		 *	Returns whether the given constant RTTI pointer is equivalent to this Datum.
		 *	Will return false if this Datum contains more than one element.
		 *	@param p a constant RTTI pointer to compare with.
		 *	@return a boolean representing whether the given RTTI pointer is equivalent to this Datum.
		 */
		bool operator==(const RTTI* p) const;

		/** Datum Table Equality Operator
		 *	Returns whether the given constant Scope reference is equivalent to this Datum.
		 *	Will return false if this Datum contains more than one element.
		 *	@param t a constant Scope reference to compare with.
		 *	@return a boolean representing whether the given Scope reference is equivalent to this Datum.
		 */
		bool operator==(const Scope& t) const;

		/** Datum Inequality Operator
		 *	Returns whether the given constant Datum reference is not equivalent to this Datum.
		 *	@param otherDatum a constant reference to the Datum to compare with.
		 *	@return a boolean representing whether the given Datum is not equivalent to this Datum.
		 */
		bool operator!=(const Datum& otherDatum) const;

		/** Datum Integer Inequality Operator
		 *	Returns whether the given constant integer reference is not equivalent to this Datum.
		 *	Will return true if this Datum contains more than one element.
		 *	@param i a constant reference to the integer to compare with.
		 *	@return a boolean representing whether the given integer is not equivalent to this Datum.
		 */
		bool operator!=(const std::int32_t i) const;

		/** Datum Float Inequality Operator
		 *	Returns whether the given constant float reference is not equivalent to this Datum.
		 *	Will return true if this Datum contains more than one element.
		 *	@param f a constant reference to the float to compare with.
		 *	@return a boolean representing whether the given float is not equivalent to this Datum.
		 */
		bool operator!=(const float f) const;

		/** Datum Vector Inequality Operator
		 *	Returns whether the given constant glm::vec4 reference is not equivalent to this Datum.
		 *	Will return true if this Datum contains more than one element.
		 *	@param v a constant reference to the glm::vec4 to compare with.
		 *	@return a boolean representing whether the given glm::vec4 is not equivalent to this Datum.
		 */
		bool operator!=(const glm::vec4 v) const;

		/** Datum Matrix Inequality Operator
		 *	Returns whether the given constant glm::mat4 reference is not equivalent to this Datum.
		 *	Will return true if this Datum contains more than one element.
		 *	@param m a constant reference to the glm::mat4 to compare with.
		 *	@return a boolean representing whether the given glm::mat4 is not equivalent to this Datum.
		 */
		bool operator!=(const glm::mat4 m) const;

		/** Datum String Inequality Operator
		 *	Returns whether the given constant string reference is not equivalent to this Datum.
		 *	Will return true if this Datum contains more than one element.
		 *	@param s a constant reference to the string to compare with.
		 *	@return a boolean representing whether the given string is not equivalent to this Datum.
		 */
		bool operator!=(const std::string s) const;

		/** Datum Pointer Inequality Operator
		 *	Returns whether the given constant RTTI pointer is not equivalent to this Datum.
		 *	Will return true if this Datum contains more than one element.
		 *	@param p a constant RTTI pointer to compare with.
		 *	@return a boolean representing whether the given RTTI pointer is not equivalent to this Datum.
		 */
		bool operator!=(const RTTI* p) const;

		/** Datum Table Inequality Operator
		 *	Returns whether the given constant Scope reference is not equivalent to this Datum.
		 *	Will return true if this Datum contains more than one element.
		 *	@param t a constant Scope reference to compare with.
		 *	@return a boolean representing whether the given Scope reference is not equivalent to this Datum.
		 */
		bool operator!=(const Scope& t) const;

		/** Datum Integer Set
		 *	Sets the Datum's value at the given index to the given integer.
		 *	Given index defaults to 0 if not specified.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's Type is not currently Datum::DatumType::Integer.
		 *	@param i a constant integer reference to set at the given location in the Datum.
		 *	@param index a constant unsigned integer representing the index into the Datum to set the given value.
		 */
		void Set(const std::int32_t& i, const std::uint32_t index = 0);

		/** Datum Float Set
		 *	Sets the Datum's value at the given index to the given float.
		 *	Given index defaults to 0 if not specified.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's Type is not currently Datum::DatumType::Float.
		 *	@param f a constant float reference to set at the given location in the Datum.
		 *	@param index a constant unsigned integer representing the index into the Datum to set the given value.
		 */
		void Set(const float& f, const std::uint32_t index = 0);

		/** Datum Vector Set
		 *	Sets the Datum's value at the given index to the given glm::vec4.
		 *	Given index defaults to 0 if not specified.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's Type is not currently Datum::DatumType::Vec4.
		 *	@param v a constant glm::vec4 reference to set at the given location in the Datum.
		 *	@param index a constant unsigned integer representing the index into the Datum to set the given value.
		 */
		void Set(const glm::vec4& v, const std::uint32_t index = 0);

		/** Datum Matrix Set
		 *	Sets the Datum's value at the given index to the given glm::mat4.
		 *	Given index defaults to 0 if not specified.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's Type is not currently Datum::DatumType::Mat4.
		 *	@param m a constant glm::mat4 reference to set at the given location in the Datum.
		 *	@param index a constant unsigned integer representing the index into the Datum to set the given value.
		 */
		void Set(const glm::mat4& m, const std::uint32_t index = 0);

		/** Datum String Set
		 *	Sets the Datum's value at the given index to the given string.
		 *	Given index defaults to 0 if not specified.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's Type is not currently Datum::DatumType::String.
		 *	@param s a constant string reference to set at the given location in the Datum.
		 *	@param index a constant unsigned integer representing the index into the Datum to set the given value.
		 */
		void Set(const std::string& s, const std::uint32_t index = 0);

		/** Datum Pointer Set
		 *	Sets the Datum's value at the given index to the given RTTI pointer.
		 *	Given index defaults to 0 if not specified.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's Type is not currently Datum::DatumType::Pointer.
		 *	@param p an RTTI pointer to set at the given location in the Datum.
		 *	@param index a constant unsigned integer representing the index into the Datum to set the given value.
		 */
		void Set(RTTI* p, const std::uint32_t index = 0);

		/** Datum Table Set
		 *	Sets the Datum's value at the given index to the given Scope pointer.
		 *	Given index defaults to 0 if not specified.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's Type is not currently Datum::DatumType::Table.
		 *	@param t a Scope reference to set at the given location in the Datum.
		 *	@param index a constant unsigned integer representing the index into the Datum to set the given value.
		 */
		void Set(Scope& t, const std::uint32_t index = 0);

		/** Datum Integer PushBack
		 *	Pushes the given constant integer reference into the Datum.
		 *	Increase the capacity of the Datum by one if more memory is needed.
		 *	Sets the Datum's type if the Datum's type was not set yet.
		 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Integer.
		 *	@exception will fail if the Datum contains external storage.
		 *	@param i a constant integer reference to push into the Datum.
		 */
		void PushBack(const std::int32_t& i);

		/** Datum Float PushBack
		 *	Pushes the given constant float reference into the Datum.
		 *	Increase the capacity of the Datum by one if more memory is needed.
		 *	Sets the Datum's type if the Datum's type was not set yet.
		 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Float.
		 *	@exception will fail if the Datum contains external storage.
		 *	@param f a constant float reference to push into the Datum.
		 */
		void PushBack(const float& f);

		/** Datum Vector PushBack
		 *	Pushes the given constant glm::vec4 reference into the Datum.
		 *	Increase the capacity of the Datum by one if more memory is needed.
		 *	Sets the Datum's type if the Datum's type was not set yet.
		 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Vec4.
		 *	@exception will fail if the Datum contains external storage.
		 *	@param v a constant glm::vec4 reference to push into the Datum.
		 */
		void PushBack(const glm::vec4& v);

		/** Datum Matrix PushBack
		 *	Pushes the given constant glm::mat4 reference into the Datum.
		 *	Increase the capacity of the Datum by one if more memory is needed.
		 *	Sets the Datum's type if the Datum's type was not set yet.
		 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Mat4.
		 *	@exception will fail if the Datum contains external storage.
		 *	@param m a constant glm::mat4 reference to push into the Datum.
		 */
		void PushBack(const glm::mat4& m);

		/** Datum String PushBack
		 *	Pushes the given constant string reference into the Datum.
		 *	Increase the capacity of the Datum by one if more memory is needed.
		 *	Sets the Datum's type if the Datum's type was not set yet.
		 *	@exception will fail if the Datum's type is not currently Datum::DatumType::String.
		 *	@exception will fail if the Datum contains external storage.
		 *	@param s a constant string reference to push into the Datum.
		 */
		void PushBack(const std::string& s);

		/** Datum Pointer PushBack
		 *	Pushes the given RTTI pointer into the Datum.
		 *	Increase the capacity of the Datum by one if more memory is needed.
		 *	Sets the Datum's type if the Datum's type was not set yet.
		 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Pointer.
		 *	@exception will fail if the Datum contains external storage.
		 *	@param p an RTTI pointer to push into the Datum.
		 */
		void PushBack(RTTI* p);

		/** Datum Table PushBack
		 *	Pushes the given Scope pointer into the Datum.
		 *	Increase the capacity of the Datum by one if more memory is needed.
		 *	Sets the Datum's type if the Datum's type was not set yet.
		 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Table.
		 *	@exception will fail if the Datum contains external storage.
		 *	@param t a Scope reference to push into the Datum.
		 */
		void PushBack(Scope& t);

		/** Datum Get<T>
		 *	Template method to get a T-typed element at the given index.
		 *	Index is defaulted to 0 if not specified.
		 *	@exception no default implementation provided.
		 */
		template<typename T>
		T Get(const std::uint32_t index = 0);

		/** Datum Constant Get<T>
		 *	Template method to get a constant T-typed element at the given index.
		 *	Index is defaulted to 0 if not specified.
		 *	@exception no default implementation provided.
		 */
		template<typename T>
		const T Get(const std::uint32_t index = 0) const;

		/** Datum SetFromString
		 *	Sets a new element of the Datum's current type at the specified index.
		 *	The new element is parsed from the given string.
		 *	Index is defaulted to 0 if not specified.
		 *	@exception will fail if the string cannot be successfully parsed as the Datum's current type.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's type is not set.
		 *	@param s a constant string reference to generate an element from.
		 *	@param index a constant unsigned integer representing the position to set to the generated element.
		 */
		void SetFromString(const std::string& s, const std::uint32_t index = 0);

		void PushBackFromString(const std::string& s);

		/** Datum ToString
		 *	Serializes the element at the specified index to a string.
		 *	Index is defaulted to 0 if not specified.
		 *	@exception will fail if the given index is out of range.
		 *	@exception will fail if the Datum's type is not set.
		 *	@param index a constant unsigned integer representing the position in the Datum to serialize into a string.
		 *	@return a string representing the desired element.
		 */
		std::string ToString(const std::uint32_t index = 0) const;

		/** Datum Reserve
		 *	Reserves space for the specified number of elements.
		 *	Does not default construct additional elements.
		 *	Cannot reduce the size of the Datum.
		 *	@exception will fail if the Datum contains external storage.
		 *	@param size a constant unsigned integer representing the number of elements to reserve space for.
		 */
		void Reserve(const std::uint32_t size);

		bool Remove(Scope& child);

		Datum(Datum&& previousDatum);

		Datum& operator=(Datum&& previousDatum);

	private:

		/** DatumValues mData for Datum
		 *	Stores the type-agnostic array of data.
		 */
		DatumValues mData;

		/** DatumType mType for Datum
		 *	Contains the current type of the Datum.
		 */
		DatumType mType;

		/** std::uint32_t mSize for Datum
		 *	Contains the current number of valid elements in the Datum.
		 */
		std::uint32_t mSize;

		/** std::uint32_t mCapacity for Datum
		 *	Contains the current potential number of elements in the Datum.
		 */
		std::uint32_t mCapacity;

		/** bool mExternal for Datum
		 *	Contains whether the Datum is holding external storage or not.
		 */
		bool mExternal;

		typedef void(Datum::*CopyElementFunction)(const Datum& otherDatum, const std::uint32_t index);
		static const HashMap<DatumType, CopyElementFunction> CopyElementFunctions;

		void CopyInt(const Datum& otherDatum, const std::uint32_t index);
		void CopyFloat(const Datum& otherDatum, const std::uint32_t index);
		void CopyVec4(const Datum& otherDatum, const std::uint32_t index);
		void CopyMat4(const Datum& otherDatum, const std::uint32_t index);
		void CopyString(const Datum& otherDatum, const std::uint32_t index);
		void CopyRTTI(const Datum& otherDatum, const std::uint32_t index);
		void CopyScope(const Datum& otherDatum, const std::uint32_t index);

		typedef void(Datum::*PushBackDefaultFunction)();
		static const HashMap<DatumType, PushBackDefaultFunction> PushBackDefaultFunctions;

		void PushBackDefaultInt();
		void PushBackDefaultFloat();
		void PushBackDefaultVec4();
		void PushBackDefaultMat4();
		void PushBackDefaultString();
		void PushBackDefaultRTTI();
		void PushBackDefaultScope();

		typedef void(Datum::*DestructorFunction)(const std::uint32_t index);
		static const HashMap<DatumType, DestructorFunction> DestructorFunctions;

		void PrimitiveDestructor(const std::uint32_t index);
		void StringDestructor(const std::uint32_t index);

		typedef bool(Datum::*IndexEqualityFunction)(const Datum& otherDatum, const std::uint32_t index) const;
		static const HashMap<DatumType, IndexEqualityFunction> IndexEqualityFunctions;

		bool IntIndexEquality(const Datum& otherDatum, const std::uint32_t index) const;
		bool FloatIndexEquality(const Datum& otherDatum, const std::uint32_t index) const;
		bool Vec4IndexEquality(const Datum& otherDatum, const std::uint32_t index) const;
		bool Mat4IndexEquality(const Datum& otherDatum, const std::uint32_t index) const;
		bool StringIndexEquality(const Datum& otherDatum, const std::uint32_t index) const;
		bool RTTIIndexEquality(const Datum& otherDatum, const std::uint32_t index) const;
		bool ScopeIndexEquality(const Datum& otherDatum, const std::uint32_t index) const;

		typedef void(Datum::*SetFromStringFunction)(const std::string& s, const std::uint32_t index);
		static const HashMap<DatumType, SetFromStringFunction> SetFromStringFunctions;

		void IntFromString(const std::string& s, const std::uint32_t index);
		void FloatFromString(const std::string& s, const std::uint32_t index);
		void Vec4FromString(const std::string& s, const std::uint32_t index);
		void Mat4FromString(const std::string& s, const std::uint32_t index);
		void StringFromString(const std::string& s, const std::uint32_t index);

		typedef void(Datum::*PushBackFromStringFunction)(const std::string& s);
		static const HashMap<DatumType, PushBackFromStringFunction> PushBackFromStringFunctions;

		void PushIntFromString(const std::string& s);
		void PushFloatFromString(const std::string& s);
		void PushVec4FromString(const std::string& s);
		void PushMat4FromString(const std::string& s);
		void PushStringFromString(const std::string& s);

		typedef std::string(Datum::*ToStringFunction)(const std::uint32_t index) const;
		static const HashMap<DatumType, ToStringFunction> ToStringFunctions;

		std::string IntToString(const std::uint32_t index) const;
		std::string FloatToString(const std::uint32_t index) const;
		std::string Vec4ToString(const std::uint32_t index) const;
		std::string Mat4ToString(const std::uint32_t index) const;
		std::string StringToString(const std::uint32_t index) const;
		std::string RTTIToString(const std::uint32_t index) const;
		std::string ScopeToString(const std::uint32_t index) const;

		static const HashMap<DatumType, size_t> DatumTypeSizes;
	};

	/** Datum Get<std::int32_t&> Template Specialization
	 *	Gets a reference to the integer at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Integer.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return an integer reference to the desired element.
	 */
	template<>
	std::int32_t& Datum::Get<std::int32_t&>(const std::uint32_t index);

	/** Datum Get<float&> Template Specialization
	 *	Gets a reference to the float at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Float.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a float reference to the desired element.
	 */
	template<>
	float& Datum::Get<float&>(const std::uint32_t index);

	/** Datum Get<glm::vec4&> Template Specialization
	 *	Gets a reference to the glm::vec4 at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Vec4.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a glm::vec4 reference to the desired element.
	 */
	template<>
	glm::vec4& Datum::Get<glm::vec4&>(const std::uint32_t index);

	/** Datum Get<glm::mat4&> Template Specialization
	 *	Gets a reference to the glm::mat4 at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Mat4.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a glm::mat4 reference to the desired element.
	 */
	template<>
	glm::mat4& Datum::Get<glm::mat4&>(const std::uint32_t index);

	/** Datum Get<std::string&> Template Specialization
	 *	Gets a reference to the string at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::String.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a string reference to the desired element.
	 */
	template<>
	std::string& Datum::Get<std::string&>(const std::uint32_t index);

	/** Datum Get<RTTI*&> Template Specialization
	 *	Gets a reference to the RTTI pointer at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Pointer.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return an RTTI pointer reference to the desired element.
	 */
	template<>
	RTTI*& Datum::Get<RTTI*&>(const std::uint32_t index);

	/** Datum Get<Scope&> Template Specialization
	 *	Gets a reference to the Scope at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Table.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a Scope reference to the desired element.
	 */
	template<>
	Scope& Datum::Get<Scope&>(const std::uint32_t index);

	/** Datum Get<const std::int32_t&> Template Specialization
	 *	Gets a constant reference to the integer at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Integer.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a constant integer reference to the desired element.
	 */
	template<>
	const std::int32_t& Datum::Get<const std::int32_t&>(const std::uint32_t index) const;

	/** Datum Get<const float&> Template Specialization
	 *	Gets a constant reference to the float at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Float.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a constant float reference to the desired element.
	 */
	template<>
	const float& Datum::Get<const float&>(const std::uint32_t index) const;

	/** Datum Get<const glm::vec4&> Template Specialization
	 *	Gets a constant reference to the glm::vec4 at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Vec4.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a constant glm::vec4 reference to the desired element.
	 */
	template<>
	const glm::vec4& Datum::Get<const glm::vec4&>(const std::uint32_t index) const;

	/** Datum Get<const glm::mat4&> Template Specialization
	 *	Gets a constant reference to the glm::mat4 at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Mat4.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a constant glm::mat4 reference to the desired element.
	 */
	template<>
	const glm::mat4& Datum::Get<const glm::mat4&>(const std::uint32_t index) const;

	/** Datum Get<const std::string&> Template Specialization
	 *	Gets a constant reference to the string at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::String.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a constant string reference to the desired element.
	 */
	template<>
	const std::string& Datum::Get<const std::string&>(const std::uint32_t index) const;

	/** Datum Get<const RTTI*&> Template Specialization
	 *	Gets a constant reference to the RTTI pointer at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Pointer.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a constant RTTI pointer reference to the desired element.
	 */
	template<>
	const RTTI*& Datum::Get<const RTTI*&>(const std::uint32_t index) const;

	/** Datum Get<const Scope&> Template Specialization
	 *	Gets a constant reference to the Scope at the specified index.
	 *	Index is defaulted to 0 if not specified.
	 *	@exception will fail if the specified index is out of range.
	 *	@exception will fail if the Datum's type is not currently Datum::DatumType::Table.
	 *	@param index constant unsigned integer representing the index to retrieve a reference to.
	 *	@return a constant Scope reference to the desired element.
	 */
	template<>
	const Scope& Datum::Get<const Scope&>(const std::uint32_t index) const;
}
