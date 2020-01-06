#pragma once
#include <vector>
#include <queue>

namespace CommonUtility
{
	template<class ObjectType, unsigned int poolSize>
	class ObjectPool
	{
	public:
		ObjectPool();
		~ObjectPool();

		ObjectType* GetFreeObject();
		void Recycle(ObjectType*& anObject);

		void ResetPool(); ///Hack function to set all pointers as free. 

	private:
		std::vector<ObjectType> myPooledObjects;
		std::queue<ObjectType*> myFreeObjects;
	};

	template<class ObjectType, unsigned int poolSize>
	inline ObjectPool<ObjectType, poolSize>::ObjectPool()
	{
		myPooledObjects.reserve(poolSize);
		myPooledObjects.resize(poolSize);
		for (unsigned int i = 0; i < poolSize; ++i)
		{
			myFreeObjects.push(&(myPooledObjects[i]));
		}
	}

	template<class ObjectType, unsigned int poolSize>
	inline ObjectPool<ObjectType, poolSize>::~ObjectPool()
	{

	}

	template<class ObjectType, unsigned int poolSize>
	inline ObjectType * ObjectPool<ObjectType, poolSize>::GetFreeObject()
	{
		if (myFreeObjects.GetSize() < 1)
		{
			assert(false && "Object pool empty. Allocate more objects");
			return nullptr;
		}

		ObjectType* toGive = myFreeObjects.front();
		myFreeObjects.pop();
		return toGive;
	}

	template<class ObjectType, unsigned int poolSize>
	inline void ObjectPool<ObjectType, poolSize>::Recycle(ObjectType*& anObject)
	{
		if (anObject == nullptr)
		{
			return;
		}

		myFreeObjects.push(anObject);
		anObject = nullptr;
	}

	template<class ObjectType, unsigned int poolSize>
	inline void ObjectPool<ObjectType, poolSize>::ResetPool()
	{
		while (!myFreeObjects.empty())
		{
			myFreeObjects.pop();
		}

		for (int i = 0; i < myPooledObjects.size(); ++i)
		{
			myFreeObjects.push(&(myPooledObjects[i]));
		}
	}
}

namespace CU = CommonUtility;