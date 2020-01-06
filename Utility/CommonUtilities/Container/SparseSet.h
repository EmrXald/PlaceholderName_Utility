#pragma once
#include <vector>
#include <assert.h>
#include <limits>

namespace CommonUtility
{
	template<class T = unsigned int>
	class SparseSet
	{
	public:
		static constexpr T failureIndex = std::numeric_limits<T>::max();

		SparseSet();
		~SparseSet();

		void Add(const T anElement);
		void RemoveCyclic(const T anElement);//Removes the element by swapping places with the last element and removing the tail of the array. Naturally changes order of elements but is faster.
		void Remove(const T anElement);//Slower than RemoveCyclic but retains relative order of dense elements
		void Clear();

		bool IsValid(const T anElement) const;

		//On found returns dense index of the element anIdentifier.
		T Find(const T anElement) const;

		void Swap(const T anElement, const T anOtherElement);

		const size_t Size() const;

		T& operator[](const size_t aDenseIndex);
		const T& operator[](const size_t aDenseIndex) const;

		void Intersection(const SparseSet<T>& anOtherSet, SparseSet<T>& anIntersectionSetOut) const;
		void ReduceToIntersection(const SparseSet<T>& anOtherSet);

	private:
		std::vector<T> myDenseList;
		std::vector<T> mySparseList;
	};

	template<class T>
	inline SparseSet<T>::SparseSet()
	{
	}

	template<class T>
	inline SparseSet<T>::~SparseSet()
	{
	}

	template<class T>
	inline void SparseSet<T>::Add(const T anElement)
	{
		assert(!IsValid(anElement) && "Identifier has already been added to sparse set before. Duplicate elements are not allowed.");

		myDenseList.push_back(anElement);
		if (mySparseList.size() < (anElement + 1))
		{
			mySparseList.reserve(anElement + 1);
		}
		while (mySparseList.size() < (anElement + 1))
		{
			mySparseList.emplace_back(failureIndex);
		}

		mySparseList[static_cast<size_t>(anElement)] = static_cast<T>(myDenseList.size() - 1);
	}

	template<class T>
	inline void SparseSet<T>::RemoveCyclic(const T anElement)
	{
		assert(IsValid(anElement) && "Element not valid, could not RemoveCyclic element.");
		const size_t lastDenseIndex = myDenseList.size() - 1;
		Swap(anElement, myDenseList[lastDenseIndex]);
		mySparseList[anElement] = failureIndex;
		myDenseList.resize(lastDenseIndex);
	}

	template<class T>
	inline void SparseSet<T>::Remove(const T anElement)
	{
		assert(IsValid(anElement) && "Element not valid, could not Remove element.");
		const T denseIndex = Find(anElement);
		myDenseList.erase(myDenseList.begin() + denseIndex);

		for (T currentDenseIndex = denseIndex; currentDenseIndex < myDenseList.size(); ++currentDenseIndex)
		{
			const T denseValue = myDenseList[currentDenseIndex];
			mySparseList[denseValue] = currentDenseIndex;
		}

		mySparseList[anElement] = failureIndex;
	}

	template<class T>
	inline void SparseSet<T>::Clear()
	{
		myDenseList.clear();
		mySparseList.clear();
	}

	template<class T>
	inline bool SparseSet<T>::IsValid(const T anElement) const
	{
		if (mySparseList.size() < (anElement + 1))
			return false;
		if (mySparseList[anElement] == failureIndex)
			return false;

		return true;
	}

	template<class T>
	inline T SparseSet<T>::Find(const T anElement) const
	{
		if (!IsValid(anElement))
			return failureIndex;

		return mySparseList[anElement];
	}


	template<class T>
	inline void SparseSet<T>::Swap(const T anElement, const T anOtherElement)
	{
		assert(IsValid(anElement) && "could not find first element for swap in sparse set!");
		assert(IsValid(anOtherElement) && "could not find second element for swap in sparse set!");

		const T sparseValue = mySparseList[anElement];
		const T otherSparseValue = mySparseList[anOtherElement];
		std::swap(myDenseList[sparseValue], myDenseList[otherSparseValue]);
		std::swap(mySparseList[anElement], mySparseList[anOtherElement]);
	}

	template<class T>
	inline const size_t SparseSet<T>::Size() const
	{
		return myDenseList.size();
	}

	template<class T>
	inline T & SparseSet<T>::operator[](const size_t aDenseIndex)
	{
		return myDenseList[aDenseIndex];
	}

	template<class T>
	inline const T & SparseSet<T>::operator[](const size_t aDenseIndex) const
	{
		return myDenseList[aDenseIndex];
	}

	template<class T>
	inline void SparseSet<T>::Intersection(const SparseSet<T>& anOtherSet, SparseSet<T>& anIntersectionSetOut) const
	{
		const SparseSet<T>& smallerSet = (Size() < anOtherSet.Size()) ? *this : anOtherSet;
		const SparseSet<T>& otherSet = ((&smallerSet) == (this)) ? anOtherSet : *this;

		for (size_t index = 0; index < smallerSet.Size(); ++index)
		{
			if (otherSet.Find(smallerSet[index]) != failureIndex)
			{
				anIntersectionSetOut.Add(smallerSet[index]);
			}
		}
	}

	template<class T>
	inline void SparseSet<T>::ReduceToIntersection(const SparseSet<T>& anOtherSet)
	{
		//SparseSet<T>& smallerSet = (Size() < anOtherSet.Size()) ? *this : const_cast<SparseSet<T>&>(anOtherSet);
		//SparseSet<T>& otherSet = ((&smallerSet) == (this)) ? const_cast<SparseSet<T>&>(anOtherSet) : *this;
		//
		//const bool thisIsSmaller = (Size() < anOtherSet.Size());
		//
		//for (size_t index = 0; index < smallerSet.Size(); ++index)
		//{
		//	T throwAway;
		//	if (!otherSet.Find(smallerSet[index], throwAway))
		//	{
		//		if (thisIsSmaller)
		//		{
		//			smallerSet.Remove(smallerSet[index]);
		//			--index;
		//		}
		//		else
		//		{
		//			otherSet.Remove(smallerSet[index]);
		//		}
		//	}
		//}

		const bool thisIsSmaller = (Size() < anOtherSet.Size());
		const SparseSet<T>& smallerSet = thisIsSmaller ? *this : anOtherSet;
		const SparseSet<T>& otherSet = thisIsSmaller ? anOtherSet : *this;

		for (size_t index = smallerSet.Size() - 1; (index > 0) && (index != failureIndex); --index)
		{
			if (otherSet.Find(smallerSet[index]) == failureIndex)
			{
				RemoveCyclic(smallerSet[index]);
			}
		}

	}
}

namespace CU = CommonUtility;