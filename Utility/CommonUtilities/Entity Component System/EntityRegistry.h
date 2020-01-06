#pragma once
#include "ComponentRegistry.h"
#include <queue>
#include <array>
#include <assert.h>

class EntityRegistry
{
public:
	EntityRegistry();
	~EntityRegistry();

	const Entity Create();
	void Destroy(const Entity& anEntity);

	template<class ComponentType>
	ComponentType& Assign(const Entity& anEntity);

	template<class ComponentType>
	void Remove(const Entity& anEntity);

	template<class ComponentType>
	const CU::SparseSet<Entity>& Get();

	template<class ... ComponentTypes>
	void Collection(CU::SparseSet<Entity>& someEntitiesOut);

private:
	ComponentRegistry myComponentRegistry;
	Entity myEntityCounter;
	std::queue<Entity> myEntityReuseQueue;

	CU::SparseSet<Entity> myInUseEntities;
};

EntityRegistry::EntityRegistry()
{
}

inline EntityRegistry::~EntityRegistry()
{
	for (size_t index = 0; index < myInUseEntities.Size(); ++index)
	{
		myComponentRegistry.RemoveAll(myInUseEntities[index]);
	}
}

inline const Entity EntityRegistry::Create()
{
	Entity entity;

	if (!myEntityReuseQueue.empty())
	{
		entity = myEntityReuseQueue.front();
		myEntityReuseQueue.pop();
	}
	else
	{
		entity = myEntityCounter++;
	}

	myInUseEntities.Add(entity);

	return entity;
}

inline void EntityRegistry::Destroy(const Entity & anEntity)
{
	myComponentRegistry.RemoveAll(anEntity);
	myEntityReuseQueue.emplace(anEntity);
	
	myInUseEntities.RemoveCyclic(anEntity);
}

template<class ComponentType>
inline ComponentType& EntityRegistry::Assign(const Entity & anEntity)
{
	return myComponentRegistry.Assign<ComponentType>(anEntity);
}

template<class ComponentType>
inline void EntityRegistry::Remove(const Entity & anEntity)
{
	myComponentRegistry.Remove<ComponentType>(anEntity);
}

template<class ComponentType>
inline const CU::SparseSet<Entity>& EntityRegistry::Get()
{
	return myComponentRegistry.GetEntities<ComponentType>();
}

template<class ...ComponentTypes>
inline void EntityRegistry::Collection(CU::SparseSet<Entity>& someEntitiesOut)
{
	if constexpr (sizeof...(ComponentTypes) == 1)
	{
		someEntitiesOut = Get<ComponentTypes...>();
	}
	else
	{
		std::array<const CU::SparseSet<Entity>*, sizeof...(ComponentTypes)> componentCollection = { &Get<ComponentTypes>()... };

		std::sort(componentCollection.begin(), componentCollection.end(), [](const CU::SparseSet<Entity>* aLHS, const CU::SparseSet<Entity>* aRHS) { return aLHS->Size() < aRHS->Size(); });

		const CU::SparseSet<Entity>& first = *componentCollection[0];
		const CU::SparseSet<Entity>& second = *componentCollection[1];

		first.Intersection(second, someEntitiesOut);

		for (size_t index = 2; index < (sizeof...(ComponentTypes)); ++index)
		{
			const CU::SparseSet<Entity>& intersectingSet = *componentCollection[index];
			someEntitiesOut.ReduceToIntersection(intersectingSet);
		}
	}
}
