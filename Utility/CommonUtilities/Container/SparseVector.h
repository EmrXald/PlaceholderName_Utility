#pragma once
#include "SparseSet.h"
#include <vector>

/*
	Vector based on indirect access through identifiers(unsigned integer types).
	Utilises a sparse set to ensure fast access of elements through identifiers(Sparse set Dense values).
	The identifiers allow persistance of reference to an element even if other elements are (re)moved.
	The use of sparse set allow (O)1 Add, Remove, Find, and Get.
	Note that while random access utilises the identifier; iteration over [] operators use raw indicies
	[0, Size()).
*/

namespace CommonUtility
{
	template<class ElementType, class SizeType = unsigned int>
	class SparseVector
	{
	public:
		static constexpr SizeType failureIndex = std::numeric_limits<SizeType>::max();

		SparseVector() {}
		~SparseVector() {}

		ElementType& Add(const SizeType anIdentifier);
		void RemoveCyclic(const SizeType anIdentifier);//Removes the element by swapping places with the last element and clipping the array tail. Identifiers remain intact but internal order of elements can change.
		void Remove(const SizeType anIsdentifier);//Slower than RemoveCyclic but retains internal order of elements in the sparse vector. Identifiers remain intact.
		//Returns the dense index of the object associated with the identifier.
		SizeType Find(const SizeType anIdentifier) const;

		ElementType& operator[](const SizeType aDenseIndex);
		const ElementType& operator[](const SizeType aDenseIndex) const;

		ElementType& Get(const SizeType anIdentifier);
		const ElementType& Get(const SizeType anIdentifier) const;

		SizeType Size() const { return static_cast<SizeType>(myElements.size()); }

	private:
		SparseSet<SizeType> mySparseIndexer;
		std::vector<ElementType> myElements;
	};

	template<class ElementType, class SizeType>
	inline ElementType& SparseVector<ElementType, SizeType>::Add(const SizeType anIdentifier)
	{
		assert(!mySparseIndexer.IsValid(anIdentifier) && "failed to add object to SparseVector(), identifier is already registered.");
		mySparseIndexer.Add(anIdentifier);
		myElements.emplace_back(ElementType());

		return myElements[myElements.size() - 1];
	}

	template<class ElementType, class SizeType>
	inline void SparseVector<ElementType, SizeType>::RemoveCyclic(const SizeType anIdentifier)
	{
		SizeType objectIndex = mySparseIndexer.Find(anIdentifier);
		assert(objectIndex != mySparseIndexer.failureIndex && "Could not RemoveCyclic on object from SparseVector, invalid identifier.");

		const SizeType lastIndex = static_cast<SizeType>(myElements.size() - 1);
		std::swap(myElements[objectIndex], myElements[lastIndex]);

		myElements.pop_back();
		mySparseIndexer.RemoveCyclic(anIdentifier);
	}

	template<class ElementType, class SizeType>
	inline void SparseVector<ElementType, SizeType>::Remove(const SizeType anIdentifier)
	{
		SizeType objectIndex = mySparseIndexer.Find(anIdentifier);
		assert(objectIndex != mySparseIndexer.failureIndex && "Could not Remove on object from SparseVector, invalid identifier.");

		myElements.erase(myElements.begin() + objectIndex);
		mySparseIndexer.Remove(anIdentifier);
	}

	template<class ElementType, class SizeType>
	inline SizeType SparseVector<ElementType, SizeType>::Find(const SizeType anIdentifier) const
	{
		return mySparseIndexer.Find(anIdentifier);
	}

	template<class ElementType, class SizeType>
	inline ElementType& SparseVector<ElementType, SizeType>::operator[](const SizeType aDenseIndex)
	{
		return myElements[aDenseIndex];
	}

	template<class ElementType, class SizeType>
	inline const ElementType& SparseVector<ElementType, SizeType>::operator[](const SizeType aDenseIndex) const
	{
		return myElements[aDenseIndex];
	}

	template<class ElementType, class SizeType>
	inline ElementType & SparseVector<ElementType, SizeType>::Get(const SizeType anIdentifier)
	{
		SizeType objectIndex = Find(anIdentifier);
		assert(objectIndex != failureIndex && "Could not Get element, identifier is invalid.");
		return myElements[objectIndex];
	}

	template<class ElementType, class SizeType>
	inline const ElementType & SparseVector<ElementType, SizeType>::Get(const SizeType anIdentifier) const
	{
		SizeType objectIndex = Find(anIdentifier);
		assert(objectIndex != failureIndex && "Could not Get element, identifier is invalid.");
		return myElements[objectIndex];
	}
}

namespace CU = CommonUtility;