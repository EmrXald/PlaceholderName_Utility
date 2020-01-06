#pragma once
#include "TemplateUtility/TemplateUtility.h"
#include "SparseVector.h"
#include <assert.h>

namespace CommonUtility
{
	template<class SizeType = unsigned int>
	class UniqueTypeMap
	{
	public:
		UniqueTypeMap()
		{
			myData = nullptr;
			myDataIterator = 0;
			myDataCapacity = 0;
		}
		~UniqueTypeMap()
		{
			if (HasInit())
			{
				for (SizeType denseIndex = 0; denseIndex < myTypes.Size(); ++denseIndex)
				{
					UniqueTypeWrapper& typeRecord = myTypes[denseIndex];
					(this->*typeRecord.myDestructFunc)(&(myData[typeRecord.myDataIndex]));
				}
				delete[] myData;
				myData = nullptr;

				myDataIterator = 0;
				myDataCapacity = 0;
			}
		}

		void Init(const SizeType anInitialCapacity);
		bool HasInit() const;
		void Reserve(const SizeType aNewCapacity);

		template<class UniqueType>
		void Add(const UniqueType& aUniqueType);

		template<class UniqueType>
		void Remove();

		template<class UniqueType>
		bool Exists() const;

		template<class UniqueType>
		UniqueType& Get();

		template<class UniqueType>
		const UniqueType& Get() const;

		template<class UniqueType>
		UniqueType* TryGet();

		template<class UniqueType>
		const UniqueType* TryGet() const;

	private:
		using FamilyType = UniqueTypeMap;
		using TypeEnumerator = TemplateUtility::TypeFamily<FamilyType>;
		using EnumeratorType = TypeEnumerator::template family_type;

		template<class UniqueType>
		void MoveData(void* aSource, void* aDestination);

		template<class UniqueType>
		void DestroyObject(void* someObjectToDestroy);

		struct UniqueTypeWrapper
		{
			void(UniqueTypeMap<SizeType>::*myMoveDataFunc)(void*, void*);
			void(UniqueTypeMap<SizeType>::*myDestructFunc)(void*);
			size_t myDataIndex;
		};

		SparseVector<UniqueTypeWrapper> myTypes;

		char* myData;
		SizeType myDataIterator;
		SizeType myDataCapacity;
	};

	template<class SizeType>
	void UniqueTypeMap<SizeType>::Init(const SizeType anInitialCapacity)
	{
		assert(!HasInit() && "UniqueTypeMap already initliased, explicitly or implicitly.");

		myData = new char[anInitialCapacity];
		myDataIterator = 0;
		myDataCapacity = anInitialCapacity;
	}

	template<class SizeType>
	bool UniqueTypeMap<SizeType>::HasInit() const
	{
		return myData != nullptr;
	}

	template<class SizeType>
	void UniqueTypeMap<SizeType>::Reserve(const SizeType aNewCapacity)
	{
		if (aNewCapacity <= myDataCapacity)
		{
			return;
		}
		if (!HasInit())
		{
			Init(aNewCapacity);
			return;
		}

		char* newData = new char[aNewCapacity];

		for (EnumeratorType typeIndex = 0; typeIndex < myTypes.Size(); ++typeIndex)
		{
			UniqueTypeWrapper& record = myTypes[typeIndex];
			(this->*record.myMoveDataFunc)(&(myData[record.myDataIndex]), &(newData[record.myDataIndex]));
		}

		delete[] myData;
		myData = newData;
		myDataCapacity = aNewCapacity;
	}

	template<class SizeType>
	template<class UniqueType>
	void UniqueTypeMap<SizeType>::Add(const UniqueType& aUniqueType)
	{
		assert(!Exists<UniqueType>() && "Type has already been added to the UniqueTypeMap, Add operation failed.");

		const SizeType newDataEnd = myDataIterator + sizeof(UniqueType);
		if (newDataEnd > myDataCapacity)
		{
			Reserve(newDataEnd);
		}

		const EnumeratorType typeIndex = TypeEnumerator::template type<UniqueType>;
		UniqueTypeWrapper& typeWrapper = myTypes.Add(typeIndex);
		typeWrapper.myMoveDataFunc = &UniqueTypeMap<SizeType>::MoveData<UniqueType>;
		typeWrapper.myDestructFunc = &UniqueTypeMap<SizeType>::DestroyObject<UniqueType>;
		typeWrapper.myDataIndex = myDataIterator;

		new(&(myData[typeWrapper.myDataIndex])) UniqueType(aUniqueType);
		myDataIterator = newDataEnd;
	}

	template<class SizeType>
	template<class UniqueType>
	inline void UniqueTypeMap<SizeType>::Remove()
	{
		assert(Exists<UniqueType>() && "Could not Remove element from UniqueTypeMap, element does not exist.");
		const EnumeratorType typeIdentifier = TypeEnumerator::template type<UniqueType>;
		const SizeType denseIndexOfRemovedElement = myTypes.Find(typeIdentifier);
		const UniqueTypeWrapper& typeRecordToRemove = myTypes[denseIndexOfRemovedElement];
		DestroyObject<UniqueType>(&(myData[typeRecordToRemove.myDataIndex]));

		for (EnumeratorType currentDenseIndex = denseIndexOfRemovedElement; (currentDenseIndex + 1) < myTypes.Size(); ++currentDenseIndex)
		{
			const UniqueTypeWrapper& previousRecord = myTypes[currentDenseIndex];

			EnumeratorType nextDenseIndex = currentDenseIndex + 1;
			UniqueTypeWrapper& recordToMove = myTypes[nextDenseIndex];

			(this->*recordToMove.myMoveDataFunc)(&(myData[recordToMove.myDataIndex]), &(myData[previousRecord.myDataIndex]));
			recordToMove.myDataIndex = previousRecord.myDataIndex;
		}

		myDataIterator = std::clamp<SizeType>(myDataIterator - sizeof(UniqueType), 0, myDataIterator);
		myTypes.Remove(typeIdentifier);
	}

	template<class SizeType>
	template<class UniqueType>
	bool UniqueTypeMap<SizeType>::Exists() const
	{
		const EnumeratorType typeIndex = TypeEnumerator::template type<UniqueType>;
		return myTypes.Find(typeIndex) != std::numeric_limits<SizeType>::max();
	}

	template<class SizeType>
	template<class UniqueType>
	inline UniqueType & UniqueTypeMap<SizeType>::Get()
	{
		const SizeType typeIdentifier = TypeEnumerator::template type<UniqueType>;
		const UniqueTypeWrapper& typeRecord = myTypes.Get(typeIdentifier);
		return *static_cast<UniqueType*>(static_cast<void*>(&(myData[typeRecord.myDataIndex])));
	}

	template<class SizeType>
	template<class UniqueType>
	inline const UniqueType & UniqueTypeMap<SizeType>::Get() const
	{
		const SizeType typeIdentifier = TypeEnumerator::template type<UniqueType>;
		const UniqueTypeWrapper& typeRecord = myTypes.Get(typeIdentifier);
		return *static_cast<UniqueType*>(static_cast<void*>(&(myData[typeRecord.myDataIndex])));
	}

	template<class SizeType>
	template<class UniqueType>
	inline UniqueType * UniqueTypeMap<SizeType>::TryGet()
	{
		if (myTypes.Find(TypeEnumerator::template type<UniqueType>) == myTypes.failureIndex)
			return nullptr;

		return &Get<UniqueType>();
	}

	template<class SizeType>
	template<class UniqueType>
	inline const UniqueType * UniqueTypeMap<SizeType>::TryGet() const
	{
		if (myTypes.Find(TypeEnumerator::template type<UniqueType>) == myTypes.failureIndex)
			return nullptr;

		return &Get<UniqueType>();
	}

	template<class SizeType>
	template<class UniqueType>
	inline void UniqueTypeMap<SizeType>::MoveData(void * aSource, void * aDestination)
	{
		new(aDestination) UniqueType(std::move(*static_cast<UniqueType*>(aSource)));
	}

	template<class SizeType>
	template<class UniqueType>
	inline void UniqueTypeMap<SizeType>::DestroyObject(void * someObjectToDestroy)
	{
		UniqueType& toDestroy = *static_cast<UniqueType*>(someObjectToDestroy);
		toDestroy.~UniqueType();
	}
}

namespace CU = CommonUtility;