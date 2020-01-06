#pragma once
#include <vector>

#include "TemplateUtility/TemplateUtility.h"
#include "Container/SparseSet.h"

typedef unsigned short Entity;

class ComponentRegistry
{
public:
	ComponentRegistry() {}
	~ComponentRegistry() 	
	{
		for (ComponentPool& p : myPools)
		{
			(this->*p.myOnDestructFunc)();
		}
	}

	template<class ComponentType>
	ComponentType& Assign(const Entity& anEntity);

	template<class ComponentType>
	void Remove(const Entity& anEntity);

	void RemoveAll(const Entity& anEntity);

	template<class ComponentType>
	std::vector<ComponentType>& GetPool();

	template<class ComponentType>
	const CU::SparseSet<Entity>& GetEntities();

	template<class ComponentType>
	const bool Exists() const;

private:

	enum class EntityEnum;

	using ComponentEnumerator = TemplateUtility::TypeFamily<EntityEnum>;
	using EnumeratorType = ComponentEnumerator::family_type;

	template<class ComponentType>
	void AssureExistance();

	template<class ComponentType>
	void OnDestruct();

	struct ComponentPool
	{
		CU::SparseSet<Entity> myEntities;
		void* myComponents = nullptr;
		void(ComponentRegistry::*myRemoveFunc)(const Entity&);
		void(ComponentRegistry::*myOnDestructFunc)();
	};

	std::vector<ComponentPool> myPools;

};

template<class ComponentType>
inline ComponentType& ComponentRegistry::Assign(const Entity & anEntity)
{
	AssureExistance<ComponentType>();

	const EnumeratorType typeIndex = ComponentEnumerator::type<ComponentType>;

	CU::SparseSet<Entity>& entities = myPools[typeIndex].myEntities;
	std::vector<ComponentType>& componentPool = GetPool<ComponentType>();
	Entity index = entities.Find(anEntity);
	assert(index == CU::SparseSet<Entity>::failureIndex && "Tried to assign a component to an entity already owning a component of that type.");
	entities.Add(anEntity);
	componentPool.push_back(ComponentType());

	return componentPool[componentPool.size() - 1];
}

template<class ComponentType>
inline void ComponentRegistry::Remove(const Entity & anEntity)
{
	AssureExistance<ComponentType>();

	const EnumeratorType typeIndex = ComponentEnumerator::type<ComponentType>;
	CU::SparseSet<Entity>& entities = myPools[typeIndex].myEntities;
	Entity entityIndex = entities.Find(anEntity);
	assert(entityIndex != CU::SparseSet<Entity>::failureIndex && "Component not found for entity, could not be removed.");

	std::vector<ComponentType>& componentPool = GetPool<ComponentType>();

	componentPool[entityIndex] = componentPool[componentPool.size() - 1];
	componentPool.resize(componentPool.size() - 1);

	entities.Remove(entityIndex);
}

template<class ComponentType>
inline std::vector<ComponentType>& ComponentRegistry::GetPool()
{
	AssureExistance<ComponentType>();

	const EnumeratorType typeIndex = ComponentEnumerator::type<ComponentType>;
	ComponentPool& pool = myPools[typeIndex];

	return *static_cast<std::vector<ComponentType>*>(pool.myComponents);
}

template<class ComponentType>
inline const CU::SparseSet<Entity>& ComponentRegistry::GetEntities()
{
	AssureExistance<ComponentType>();
	const EnumeratorType typeIndex = ComponentEnumerator::type<ComponentType>;
	const ComponentPool& pool = myPools[typeIndex];

	return pool.myEntities;
}

template<class ComponentType>
inline const bool ComponentRegistry::Exists() const
{
	const EnumeratorType typeIndex = ComponentEnumerator::type<ComponentType>;
	return (typeIndex < myPools.size());
}

template<class ComponentType>
inline void ComponentRegistry::AssureExistance()
{
	if (Exists<ComponentType>())
	{
		return;
	}

	const EnumeratorType typeIndex = ComponentEnumerator::type<ComponentType>;

	myPools.resize(typeIndex + 1);

	ComponentPool& newPool = myPools[typeIndex];
	newPool.myComponents = new std::vector<ComponentType>();

	newPool.myRemoveFunc = &ComponentRegistry::Remove<ComponentType>;
	newPool.myOnDestructFunc = &ComponentRegistry::OnDestruct<ComponentType>;
}

template<class ComponentType>
inline void ComponentRegistry::OnDestruct()
{
	std::vector<ComponentType>* componentsToDelete = &GetPool<ComponentType>();
	delete componentsToDelete;
}

inline void ComponentRegistry::RemoveAll(const Entity & anEntity)
{
	for (int i = 0; i < myPools.size(); ++i)
	{
		ComponentPool& currentPool = myPools[i];
		if (currentPool.myEntities.Find(anEntity) != currentPool.myEntities.failureIndex)
		{
			(this->*currentPool.myRemoveFunc)(anEntity);
		}
	}
}