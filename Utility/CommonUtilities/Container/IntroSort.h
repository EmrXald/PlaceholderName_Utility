#pragma once
#include <vector>
#include <functional>

namespace CommonUtility
{
	template<class T>
	void InsertionSort(std::vector<T>& aVec, const size_t aBegin, const size_t anEnd, const std::function<const bool(const T&, const T&)>& aCompFunc, const size_t aVecSize)
	{
		size_t i, j;
		T tempStore;
		for (i = aBegin + 1; i < anEnd; ++i)
		{
			tempStore = aVec[i];
			j = i - 1;
			while ((j < aVecSize) && (j >= aBegin) && aCompFunc(tempStore, aVec[j]))
			{
				aVec[j + 1] = aVec[j];
				--j;
			}
			aVec[j + 1] = tempStore;
		}
	}

	template<class T>
	const size_t MedianOfThree(const std::vector<T>& aVec, const size_t aFirst, const size_t aSecond, const size_t aLast)
	{
		if (aVec[aFirst] < aVec[aSecond] && aVec[aSecond] < aVec[aLast])
			return aSecond;

		if (aVec[aFirst] < aVec[aLast] && aVec[aLast] <= aVec[aSecond])
			return aLast;

		if (aVec[aSecond] <= aVec[aFirst] && aVec[aFirst] < aVec[aLast])
			return aFirst;

		if (aVec[aSecond] < aVec[aLast] && aVec[aLast] <= aVec[aFirst])
			return aLast;

		if (aVec[aLast] <= aVec[aFirst] && aVec[aFirst] < aVec[aSecond])
			return aFirst;

		if (aVec[aLast] <= aVec[aSecond] && aVec[aSecond] <= aVec[aLast])
			return aSecond;

		return aFirst;
	}

	template<class T>
	inline const size_t Partition2(std::vector<T>& aVec, const size_t aBegin, const size_t anEnd, const std::function<const bool(const T&, const T&)>& aCompFunc)
	{
		const auto& pivot = aVec[anEnd];
		size_t i = (aBegin - 1);

		for (size_t j = aBegin; j <= anEnd - 1; j++)
		{
			if (aCompFunc(aVec[j], pivot))
			{
				i++;
				std::swap(aVec[i], aVec[j]);
			}
		}
		std::swap(aVec[i + 1], aVec[anEnd]);
		return (i + 1);
	}

	template<class T>
	void BuildHeap(std::vector<T>& aVec, const size_t anEnd, const size_t aStart, const std::function<const bool(const T&, const T&)>& aCompFunc)
	{
		size_t risingIndex = aStart;
		size_t left = 2 * aStart + 1;
		size_t right = 2 * aStart + 2;

		if ((left < anEnd) && aCompFunc(aVec[risingIndex], aVec[left]))
		{
			risingIndex = left;
		}

		if ((right < anEnd) && aCompFunc(aVec[risingIndex], aVec[right]))
		{
			risingIndex = right;
		}

		if (risingIndex != aStart)
		{
			std::swap(aVec[aStart], aVec[risingIndex]);
			BuildHeap<T>(aVec, anEnd, risingIndex, aCompFunc);
		}
	}

	template<class T>
	inline void HeapSort(std::vector<T>& aVec, const size_t aBegin, const size_t anEnd, const std::function<const bool(const T&, const T&)>& aCompFunc, const size_t aVecSize)
	{
		// Build heap (rearrange array) 
		for (size_t i = (anEnd / 2) - 1; (i >= aBegin) && (i < aVecSize); i--)
		{
			BuildHeap<T>(aVec, anEnd, i, aCompFunc);
		}

		// One by one extract an element from heap 
		for (size_t i = anEnd - 1; (i >= aBegin) && (i < aVecSize); i--)
		{
			std::swap(aVec[aBegin], aVec[i]);

			BuildHeap<T>(aVec, i, aBegin, aCompFunc);
		}
	}

	template<class T>
	inline const size_t Partition(std::vector<T>& aVec, const size_t aLowerBound, const size_t aUpperBound)
	{
		const size_t pivotIndex = aLowerBound + (aUpperBound - aLowerBound) / 2;
		std::swap(aVec[pivotIndex], aVec[aLowerBound]);
		const auto& pivotElement = aVec[aLowerBound];

		size_t i = aLowerBound + 1;
		size_t j = aUpperBound;

		while (i <= j)
		{
			while ((i <= j) && (aVec[i] <= pivotElement))
			{
				i++;
			}

			while ((i <= j) && (aVec[j] > pivotElement))
			{
				j--;
			}

			if (i < j)
			{
				std::swap(aVec[i], aVec[j]);
			}
		}

		std::swap(aVec[i - 1], aVec[aLowerBound]);
		return i - 1;
	}

	template<class T>
	inline void IntroSortImpl(std::vector<T>& aVec, const size_t aBegin, const size_t anEnd, const std::function<const bool(const T&, const T&)>& aCompFunc, const int aDepthLim, const size_t aVecSize)
	{
		size_t size = anEnd - aBegin;
		if (size > aVecSize)
		{
			return;
		}
		if (size <= 2)//insertion sort threshold.
		{
			InsertionSort<T>(aVec, aBegin, anEnd, aCompFunc, aVecSize);
			return;
		}

		if (aDepthLim <= 0)
		{
			HeapSort<T>(aVec, aBegin, anEnd + 1, aCompFunc, aVecSize);
			return;
		}

		const size_t pivot = MedianOfThree<T>(aVec, aBegin, aBegin + (size / 2), anEnd);

		std::swap(aVec[pivot], aVec[anEnd]);

		const size_t partitionPoint = Partition2<T>(aVec, aBegin, anEnd, aCompFunc);
		IntroSortImpl<T>(aVec, aBegin, partitionPoint - 1, aCompFunc, aDepthLim - 1, aVecSize);
		IntroSortImpl<T>(aVec, partitionPoint + 1, anEnd, aCompFunc, aDepthLim - 1, aVecSize);
	}

	template<class T>
	inline void IntroSort(std::vector<T>& aVec, const size_t aBegin, const size_t anEnd, const std::function<const bool(const T&, const T&)>& aCompFunc)
	{
		int depthLimit = static_cast<int>(1.5 * std::log(anEnd - aBegin));
		IntroSortImpl<T>(aVec, aBegin, anEnd - 1, aCompFunc, depthLimit, aVec.size());
	}

	template<class T>
	inline void Quicksort(std::vector<T>& aVec, const size_t aBegin, const size_t anEnd)
	{
		if ((aBegin >= anEnd) || (anEnd >= aVec.size()))
		{
			return;
		}

		const size_t partitionIndex = Partition<T>(aVec, aBegin, anEnd);
		Quicksort<T>(aVec, aBegin, partitionIndex - 1);
		Quicksort<T>(aVec, partitionIndex + 1, anEnd);
	}
}

namespace CU = CommonUtility;