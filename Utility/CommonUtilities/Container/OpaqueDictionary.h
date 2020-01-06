#pragma once
#include <map>
#include <typeinfo>
#include <new>
#include <assert.h>
namespace CommonUtility
{
	template<class Key, unsigned int bucketSize>
	class Dictionary
	{
	public:
		Dictionary();
		~Dictionary();

		template<class Type>
		Type& GetValue(const Key& aKey);

		template<class Type>
		void SetValue(const Key& aKey, const Type& aValue);

	private:
		bool ValueExists(const Key& aKey);

		char myDataBucket[bucketSize];
		unsigned int myIndex;
		std::map<Key, void*> myData;
		std::map<Key, const std::type_info* > myDataTypes;
	};

	template<class Key, unsigned int bucketSize>
	inline Dictionary<Key, bucketSize>::Dictionary()
	{
		memset(myDataBucket, 0, bucketSize);
		myIndex = 0;
	}

	template<class Key, unsigned int bucketSize>
	Dictionary<Key, bucketSize>::~Dictionary()
	{
		memset(myDataBucket, 0, bucketSize);
		myIndex = 0;
	}

	template<class Key, unsigned int bucketSize>
	inline bool Dictionary<Key, bucketSize>::ValueExists(const Key & aKey)
	{
		std::map<Key, void*>::iterator toFind = myData.find(aKey);

		if (toFind == myData.end())
		{
			return false;
		}

		return true;
	}

	template<class Key, unsigned int bucketSize>
	template<class Type>
	inline Type& Dictionary<Key, bucketSize>::GetValue(const Key & aKey)
	{
		if (!ValueExists(aKey))
		{
			return nullptr;
		}

		assert((*myDataTypes[aKey]) == typeid(Type) && "Type of GetValue for dictionary does not match, request proper type.");
		return *static_cast<Type*>(myData[aKey]);
	}

	template<class Key, unsigned int bucketSize>
	template<class Type>
	inline void Dictionary<Key, bucketSize>::SetValue(const Key & aKey, const Type & aValue)
	{
		if (ValueExists(aKey))
		{
			Type& toReplace = GetValue<Type>(aKey);
			toReplace = aValue;
			return;
		}

		assert((myIndex + sizeof(Type)) <= bucketSize && "Dictionary has reached maximum capacity. Writing more data results in memory corruption.");
		//a void pointer to the memory is saved in myData with the given key using an std::map.
		myData[aKey] = new(&myDataBucket[myIndex]) Type(aValue);
		myDataTypes[aKey] = &typeid(Type);

		//Moves free index of the char array by size of given Type, thus why char is used, as it has a size of one byte.
		myIndex += sizeof(Type);
	}
}

namespace CU = CommonUtility;
