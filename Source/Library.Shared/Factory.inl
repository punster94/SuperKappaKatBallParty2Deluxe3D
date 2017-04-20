#pragma once
#include "pch.h"

namespace FieaGameEngine
{

#pragma region

	template <class AbstractProductT>
	Factory<AbstractProductT>* Factory<AbstractProductT>::Find(const std::string& className)
	{
		Iterator it = sFactoryMap.Find(className);

		if (it == end())
		{
			return nullptr;
		}

		return it->second;
	}

	template <class AbstractProductT>
	AbstractProductT* Factory<AbstractProductT>::Create(const std::string& className)
	{
		Factory<AbstractProductT>* factory = Find(className);

		if (factory == nullptr)
		{
			return nullptr;
		}

		return factory->Create();
	}

	template <class AbstractProductT>
	typename Factory<AbstractProductT>::Iterator Factory<AbstractProductT>::begin()
	{
		return sFactoryMap.begin();
	}

	template <class AbstractProductT>
	typename Factory<AbstractProductT>::Iterator Factory<AbstractProductT>::end()
	{
		return sFactoryMap.end();
	}

	template <class AbstractProductT>
	void Factory<AbstractProductT>::Add(Factory<AbstractProductT>* factory)
	{
		Factory<AbstractProductT>* foundFactory = Find(factory->GetClassName());

		if (foundFactory == nullptr)
		{
			sFactoryMap[factory->GetClassName()] = factory;
		}
	}

	template <class AbstractProductT>
	void Factory<AbstractProductT>::Remove(Factory<AbstractProductT>* factory)
	{
		if (Find(factory->GetClassName()) == factory)
		{
			sFactoryMap.Remove(factory->GetClassName());
		}
	}

	template <class AbstractProductT>
	typename Factory<AbstractProductT>::FactoryMapType Factory<AbstractProductT>::sFactoryMap;

#pragma endregion Factory<AbstractProductT> Manager Singleton Implementation

}