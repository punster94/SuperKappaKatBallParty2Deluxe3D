#pragma once

#include "Scope.h"

namespace FieaGameEngine
{

	/** Attributed class
	 *	A class derived from Scope that provides useful data wrapping for managing attributes of an instance.
	 */
	class Attributed : public Scope
	{

		RTTI_DECLARATIONS(Attributed, Scope)

	public:

		/** Attributed Default Constructor
		 *	Calls the parent Scope constructor with the given initial size.
		 *	@param initialSize the unsigned integer size to initialize the structure to.
		 */
		explicit Attributed(std::uint32_t initialSize = 13);

		/** Attributed Destructor
		 *	Polymorphically calls the parent Scope destructor.
		 *	Defaulted because Attributed needs no extra destruction beyond it's parents functionality.
		 */
		virtual ~Attributed() = default;

		/** Attributed Copy Constructor
		 *	Copies the given Attributed reference into this Attributed.
		 *	@param otherAttributed a constant reference to another Attributed to copy.
		 */
		Attributed(const Attributed& otherAttributed);

		/** Attributed Assignment Operator
		 *	Copies the given Attributed reference into this Attributed.
		 *	@param otherAttributed a constant reference to another Attributed to copy.
		 *	@return an Attributed reference representing this Attributed.
		 */
		Attributed& operator=(const Attributed& otherAttributed);

		/** Attributed Move Constructor
		 *	Copies the given Attributed reference into this Attributed.
		 *	Clears out the passed in Attributed.
		 *	@param previousAttributed the Attributed double reference to clear after copying.
		 */
		Attributed(Attributed&& previousAttributed);

		/** Attributed Move Assignment Operator
		 *	Copies the given Attributed reference into this Attributed.
		 *	Clears out the passed in Attributed.
		 *	@param previousAttributed the Attributed double reference to clear after copying.
		 *	@return an Attributed reference representing this Attributed.
		 */
		Attributed& operator=(Attributed&& previousAttributed);

		/** Attributed Equality Operator
		 *	Determines whether the given Attributed is equivalent to this Attributed.
		 *	@param otherAttributed the constant Attributed reference to compare this Attributed to.
		 *	@return whether the given Attributed is equivalent to this Attributed.
		 */
		bool operator==(const Attributed& otherAttributed) const;

		/** Attributed Inequality Operator
		 *	Determines whether the given Attributed is not equivalent to this Attributed.
		 *	@param otherAttributed the constant Attributed reference to compare this Attributed to.
		 *	@return whether the given Attributed is not equivalent to this Attributed.
		 */
		bool operator!=(const Attributed& otherAttributed) const;

		/** Attributed Equals
		 *	Determines whether the given RTTI pointer is equivalent to this Attributed.
		 *	@param otherAttributed the constant RTTI pointer to compare this Attributed to.
		 *	@return whether the given RTTI pointer is equivalent to this Attributed.
		 */
		bool Equals(const RTTI* otherAttributed) const override;

		/** Attributed IsPrescribedAttribute
		 *	Determines whether the given attribute name is a prescribed attribute of this Attributed.
		 *	@param key a constant string reference that represents the name of the attribute to look for.
		 *	@return a boolean representing whether the given attribute name is a prescribed attribute of this Attributed.
		 */
		bool IsPrescribedAttribute(const std::string& key) const;

		/** Attributed IsAuxiliaryAttribute
		 *	Determines whether the given attribute name is an auxiliary attribute of this Attributed.
		 *	@param key a constant string reference that represents the name of the attribute to look for.
		 *	@return a boolean representing whether the given attribute name is an auxiliary attribute of this Attributed.
		 */
		bool IsAuxiliaryAttribute(const std::string& key) const;

		/** Attributed IsAttribute
		 *	Determines whether the given attribute name is an attribute of this Attributed.
		 *	@param key a constant string reference that represents the name of the attribute to look for.
		 *	@return a boolean representing whether the given attribute name is an attribute of this Attributed.
		 */
		bool IsAttribute(const std::string& key) const;

		/** Attributed AppendAuxiliaryAttribute
		 *	Appends an attribute to this Attributed with the given name, adding one to the list if the attribute exists.
		 *	@exception will fail if the attribute already exists as a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute.
		 *	@return a Datum reference representing the attribute that was created or appended to.
		 */
		Datum& AppendAuxiliaryAttribute(const std::string& key);

		/** Attributed ClearPrescribedAttributes
		 *	A utility method that clears all prescribed attributes in the static attribute list.
		 *	Used to prevent memory leaks.
		 */
		static void ClearPrescribedAttributes();

		/** Attributed CopyAuxiliaryAttributesInto
		 *	Copies the auxiliary attributes of this Attributed into the given Attributed.
		 *	@param otherAttributed an Attributed reference to append this Attributed instances auxiliary attributes into.
		 */
		void CopyAuxiliaryAttributesInto(Attributed& otherAttributed) const;

	protected:

		/** Attributed InitializeSignatures
		 *	Provides the interface for classes that will inherit from Attributed to initialize their prescribed attributes.
		 *	Explicitly deleted.
		 */
		void InitializeSignatures();

		/** Attributed AddInternalAttribute for integer
		 *	Adds a prescribed internal attribute of type integer.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param size a constant unsigned integer representing the number of elements of the attribute.
		 *	@param initialValue an unsigned integer to default added fields in the new attribute.
		 */
		void AddInternalAttribute(const std::string& key, const std::uint32_t size, std::int32_t initialValue = 0);

		/** Attributed AddInternalAttribute for float
		 *	Adds a prescribed internal attribute of type float.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param size a constant unsigned integer representing the number of elements of the attribute.
		 *	@param initialValue a float to default added fields in the new attribute.
		 */
		void AddInternalAttribute(const std::string& key, const std::uint32_t size, float initialValue = 0.0);

		/** Attributed AddInternalAttribute for vector
		 *	Adds a prescribed internal attribute of type vec4.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param size a constant unsigned integer representing the number of elements of the attribute.
		 *	@param initialValue a glm::vec4 reference to default added fields in the new attribute.
		 */
		void AddInternalAttribute(const std::string& key, const std::uint32_t size, glm::vec4& initialValue);

		/** Attributed AddInternalAttribute for matrix
		 *	Adds a prescribed internal attribute of type matrix.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param size a constant unsigned integer representing the number of elements of the attribute.
		 *	@param initialValue a glm::mat4 reference to default added fields in the new attribute.
		 */
		void AddInternalAttribute(const std::string& key, const std::uint32_t size, glm::mat4& initialValue);

		/** Attributed AddInternalAttribute for string
		 *	Adds a prescribed internal attribute of type string.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param size a constant unsigned integer representing the number of elements of the attribute.
		 *	@param initialValue a string reference to default added fields in the new attribute.
		 */
		void AddInternalAttribute(const std::string& key, const std::uint32_t size, std::string& initialValue);

		/** Attributed AddInternalAttribute for pointer
		 *	Adds a prescribed internal attribute of type RTTI*.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param size a constant unsigned integer representing the number of elements of the attribute.
		 *	@param initialValue an RTTI pointer to default added fields in the new attribute.
		 */
		void AddInternalAttribute(const std::string& key, const std::uint32_t size, RTTI* initialValue = nullptr);

		/** Attributed AddExternalAttribute for integers
		 *	Adds a prescribed integer attribute of the given size.
		 *	Sets the attribute to use the external data provided.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param data the external data to set the added attribute to.
		 *	@param size a constant unsigned integer representing the number of elements that should be in the added attribute.
		 */
		void AddExternalAttribute(const std::string& key, std::int32_t* data, const std::uint32_t size = 1);

		/** Attributed AddExternalAttribute for floats
		 *	Adds a prescribed float attribute of the given size.
		 *	Sets the attribute to use the external data provided.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param data the external data to set the added attribute to.
		 *	@param size a constant unsigned integer representing the number of elements that should be in the added attribute.
		 */
		void AddExternalAttribute(const std::string& key, float* data, const std::uint32_t size = 1);

		/** Attributed AddExternalAttribute for vectors
		 *	Adds a prescribed vector attribute of the given size.
		 *	Sets the attribute to use the external data provided.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param data the external data to set the added attribute to.
		 *	@param size a constant unsigned integer representing the number of elements that should be in the added attribute.
		 */
		void AddExternalAttribute(const std::string& key, glm::vec4* data, const std::uint32_t size = 1);

		/** Attributed AddExternalAttribute for matrix
		 *	Adds a prescribed matrix attribute of the given size.
		 *	Sets the attribute to use the external data provided.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param data the external data to set the added attribute to.
		 *	@param size a constant unsigned integer representing the number of elements that should be in the added attribute.
		 */
		void AddExternalAttribute(const std::string& key, glm::mat4* data, const std::uint32_t size = 1);

		/** Attributed AddExternalAttribute for strings
		 *	Adds a prescribed string attribute of the given size.
		 *	Sets the attribute to use the external data provided.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param data the external data to set the added attribute to.
		 *	@param size a constant unsigned integer representing the number of elements that should be in the added attribute.
		 */
		void AddExternalAttribute(const std::string& key, std::string* data, const std::uint32_t size = 1);

		/** Attributed AddExternalAttribute for RTTI pointers
		 *	Adds a prescribed pointer attribute of the given size.
		 *	Sets the attribute to use the external data provided.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the attribute to add.
		 *	@param data the external data to set the added attribute to.
		 *	@param size a constant unsigned integer representing the number of elements that should be in the added attribute.
		 */
		void AddExternalAttribute(const std::string& key, RTTI** data, const std::uint32_t size = 1);

		/** Attributed AddNestedScope
		 *	Adds a prescribed table attribute by appending a Scope to this Attributed.
		 *	Will perform no action if the attribute is already a prescribed attribute.
		 *	@param key a constant string reference representing the name of the table attribute to add.
		 *	@param scope a Scope reference representing the scope to add to this Attributed classes prescribed attribute list.
		 */
		void AddNestedScope(const std::string& key, Scope& scope);

		/** Attributed AddEmptyTable
		 *	Adds a prescribed table attribute by appending an empty Datum of type Table.
		 *	@param key a constant string reference representing the name of the table attribute to add.
		 */
		void AddEmptyTable(const std::string& key);

		void AddEmptyString(const std::string& key);

		/** Attributed FixExternalAttributes
		 *	Method intended to implement necessary logic for fixing up external references to this Attributed's member data.
		 *	Should be called after copying or moving into this Attributed.
		 */
		void FixExternalAttributes();

	private:

		/** Attributed AddPrescribedAttributeKey
		 *	Adds the given key to the prescribed attribute list.
		 *	@param key a constant string reference representing the key to be added to the prescribed attributes list.
		 */
		void AddPrescribedAttributeKey(const std::string& key);

		/** Attributed mPrescribedAttributeList
		 *	A static HashMap<std::uint64_t, Vector<std::string>> that holds each Attributed classes list of prescribed attributes for each Attributed instances reference.
		 */
		static HashMap<std::uint64_t, Vector<std::string>> mPrescribedAttributeList;

	public:

		/** Attributed sAttributedThisKey
		 *	A static constant string that represents the key in any Attributed instance that the this pointer exists in.
		 */
		static const std::string sAttributedThisKey;

	};
}