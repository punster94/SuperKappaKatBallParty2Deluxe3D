#pragma once

#include <string>
#include "HashMap.h"

namespace FieaGameEngine
{
	/** Factory Templated Class
	 *	A class to be used as both manager and instance implementing the factory pattern.
	 *	Derived classes are implemented through the ConcreteFactory(BaseT, DerivedT) macro.
	 */
	template <class AbstractProductT>
	class Factory
	{

#pragma region

	public:

		/** Factory FactoryMapType Type Definition
		 *	Defines the type of HashMap stored statically in the Factory that contains Factory pointers.
		 */
		typedef HashMap<std::string, Factory<AbstractProductT>*> FactoryMapType;

		/** Factory Iterator Type Definition
		 *	Defines the Iterator type for this Factory.
		 */
		typedef typename FactoryMapType::Iterator Iterator;

		/** Factory GetClassName
		 *	A virtual method that returns the string representing this Factory's product type.
		 *	Explicitly deleted.
		 *	@return a constant character pointer that represents this Factory's product type.
		 */
		virtual const std::string GetClassName() const = 0;

	private:

		/** Factory Instance Create
		 *	A virtual method that returns a new object of the type the Factory can create.
		 *	Explicitly deleted.
		 *	@return a pointer to the abstract base class the product of the Factory is derived from.
		 */
		virtual AbstractProductT* Create() const = 0;

#pragma endregion Interface (Abstract Base Class)

#pragma region

	public:

		/** Factory Find
		 *	A static method for finding a Factory that produces a given product type as a string.
		 *	@param className a constant character pointer representing the type of product the returned Factory can create.
		 *	@return a Factory pointer that can create a product of the specified type or nullptr if no factory found.
		 */
		static Factory<AbstractProductT>* Find(const std::string& className);

		/** Factory Create
		 *	A static method for creating a product whose type is specified as a string.
		 *	@param className a constant character pointer representing the type of product to create.
		 *	@return a base class pointer to the newly created product of the specified type.
		 */
		static AbstractProductT* Create(const std::string& className);

		/** Factory begin
		 *	A static method for getting the first entry in the factory HashMap.
		 *	@return an Iterator that represents the first entry in the HashMap of Factories.
		 */
		static typename Iterator begin();

		/** Factory end
		 *	A static method for getting the entry after the last in the factory HashMap.
		 *	@return an Iterator that represents the entry after the last in the HashMap of Factories.
		 */
		static typename Iterator end();

	protected:

		/** Factory Add
		 *	A static method for adding a given Factory pointer to the static HashMap of Factory pointers.
		 *	@param factory a Factory pointer to be added to the HashMap.
		 */
		static void Add(Factory<AbstractProductT>* factory);

		/** Factory Remove
		 *	A static method for removing a given Factory pointer from the static HashMap of Factory pointers.
		 *	@param factory a Factory pointer to be removed from the HashMap.
		 */
		static void Remove(Factory<AbstractProductT>* factory);

	private:

		/** Factory sFactoryMap
		 *	A static HashMap of Factory pointers mapped by a string representing the type of product they can create.
		 */
		static FactoryMapType sFactoryMap;

#pragma endregion Manager (Singleton)

	};

#pragma region

	/** CreateFactory Macro
	 *	Generates the Factory class used to create the given concrete product.
	 */
#define ConcreteFactory(AbstractProductT, ConcreteProductT)								\
	class ConcreteProductT##Factory : public FieaGameEngine::Factory<AbstractProductT>	\
	{																					\
	public:																				\
		ConcreteProductT##Factory() { Add(this); }										\
		~ConcreteProductT##Factory() { Remove(this); }									\
		virtual const std::string GetClassName(void) const								\
		{																				\
			return #ConcreteProductT;													\
		}																				\
		virtual AbstractProductT* Create(void) const									\
		{																				\
			AbstractProductT* product = new ConcreteProductT();							\
			assert(product != NULL);													\
			return product;																\
		}																				\
	}

#pragma endregion ConcreteFactory Macro Definition

}

#include "Factory.inl"