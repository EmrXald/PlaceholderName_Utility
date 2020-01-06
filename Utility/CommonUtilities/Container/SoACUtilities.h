#pragma once
#include "SoAC.h"
#include "TemplateUtility/ChooseType.h"
#include "TemplateUtility/TypeTraits.h"
#define ISORT_MAX 32 //maximum size for insertion sort.

//Since all iterator logic works as an indirection to indices indicises can be used(in my case at least).
//But with iterators we have direct pointer access to the heap array and circumvent index accessing, most likely faster.

namespace CommonUtility
{
	template<class...TypeList>
	struct TupleUtil
	{
	public:
		static std::tuple<TypeList&...> ConstructReference(std::tuple<TypeList...>& aTuple)
		{
			return ConstructReferenceImpl(aTuple, std::make_index_sequence<sizeof...(TypeList)>{});
		}
	private:

		template <size_t ... IndexSequence>
		static std::tuple<TypeList&...> ConstructReferenceImpl(std::tuple<TypeList...>& aTuple, const std::index_sequence<IndexSequence...>&)
		{
			return std::tuple<TypeList&...>(std::get<IndexSequence>(aTuple)...);
		}
	};


	template<class ... TypeList>
	struct ISort
	{
	public:
		using PredicateSignature = std::function<const bool(const std::tuple<const TypeList&...>&, const std::tuple<const TypeList&...>&)>;

		ISort(SoAC<TypeList...>& aContainerToSort, size_t aFirst, size_t aLast, const PredicateSignature& aPredicate) : myContainerToSort(aContainerToSort)
		{
			IntroSort(aFirst, aLast, aLast - aFirst, aPredicate);
		}

	private:

		inline void IntroSort(size_t aFirst, size_t aLast, size_t anIdeal, const PredicateSignature& aPredicate);
		inline std::pair<size_t, size_t> PartitionByMedianGuess(size_t aFirst, size_t aLast, const PredicateSignature& aPredicate);
		inline void GuessMedian(size_t aFirst, size_t aMid, size_t aLast, const PredicateSignature& aPredicate);
		inline void MedianOfThree(size_t aFirst, size_t aMid, size_t aLast, const PredicateSignature& aPredicate);
		inline void HeapSort(size_t aFirst, size_t aLast, const PredicateSignature& aPredicate);
		inline void BuildHeap(size_t aFirst, size_t aLast, const PredicateSignature& aPredicate);
		inline void InsertionSort(size_t aFirst, size_t aLast, const PredicateSignature& aPredicate);

		SoAC<TypeList...>& myContainerToSort;

	};

	//Intro sort implementation using indices instead of iterators. allows for complex predicate but will be slower. TODO: Implement intro sort with iterators, where the predicate only cares about a single type.
	template<class ... TypeList>
	inline void ISort<TypeList...>::IntroSort(size_t aFirst, size_t aLast, size_t anIdeal, const PredicateSignature& aPredicate)
	{
		size_t count;
		while ((ISORT_MAX < (count = aLast - aFirst)) && (0 < anIdeal))
		{
			//PartitionByMedianGuess returns lower and upper bounds from partition.
			std::pair<size_t, size_t> midBounds = PartitionByMedianGuess(aFirst, aLast, aPredicate);
			anIdeal = (anIdeal >> 1) + (anIdeal >> 2);//essenatially anIdeal = (anIdeal/2) + (anIdeal/4);
			//if the lower bound ranges are smaller we loop over and sort them first.
			if (midBounds.first - aFirst < aLast - midBounds.second)
			{
				IntroSort(aFirst, midBounds.first, anIdeal, aPredicate);
				aFirst = midBounds.second;
			}
			else
			{
				IntroSort(midBounds.second, aLast, anIdeal, aPredicate);
				aLast = midBounds.first;
			}
		}
		//Assumption that intro sorting will not be faster, resolve to heap sorting.
		if (ISORT_MAX < count)
		{
			HeapSort(aFirst, aLast, aPredicate);//resolve to own implementation for now. look at std later.
		}
		else if (2 <= count)//insertion sort used
		{
			InsertionSort(aFirst, aLast, aPredicate);//resolve to own implementation for now. look at std later.
		}
	}

	//partition [aFirst,aLast) using aPredicate. aka from aFirst(inclusive) to aLast(Exclusive).
	template<class ... TypeList>
	inline std::pair<size_t, size_t> ISort<TypeList...>::PartitionByMedianGuess(size_t aFirst, size_t aLast, const PredicateSignature& aPredicate)
	{
		size_t mid = aFirst + ((aLast - aFirst) >> 1);//Here std uses bitshift instead of division.
		GuessMedian(aFirst, mid, aLast - 1, aPredicate);//poor naming, actually does a median guess then sorts that elment to the middle.
		size_t first = mid;
		size_t last = first + 1;//element after first(mid?).
		//these two while loops will find the bounds for us and we will de the partitioning later.
		//first is greater than aFirst(lower bound) and predicate of element to the left is not fulfilled.
		while ((aFirst < first)
			&& (!aPredicate(myContainerToSort[first - 1], myContainerToSort[first]))
			&& (!aPredicate(myContainerToSort[first], myContainerToSort[first - 1])))
		{
			--first;//decrement lower bound iterator.
		}
		//index last is lower than our upper bound aLast and the predicate is NOT fulfilled.
		//extend last from inital mid+1 to partition a greater set.
		while ((last < aLast)
			&& !aPredicate(myContainerToSort[last], myContainerToSort[first])
			&& !aPredicate(myContainerToSort[first], myContainerToSort[last]))
		{
			++last;//increment upper bound iterator.
		}

		size_t gFirst = last;//taking on naming convention of std because I can't find a fitting name.
		size_t gLast = first;

		for (;;)//basically a while(true) loop, nice.
		{
			for (; gFirst < aLast; ++gFirst)
			{
				if (aPredicate(myContainerToSort[first], myContainerToSort[gFirst]))
				{

				}
				else if (aPredicate(myContainerToSort[gFirst], myContainerToSort[first]))//if iterator(gFirst) and lowerbound(first) fulfill pred. ignore
				{
					break;
				}
				else if (last != gFirst) //predicate not fulfilled, swap and move upper bound.
				{
					myContainerToSort.Swap(last, gFirst);
					++last;
				}
				else //move upper bound
				{
					++last;
				}
			}

			for (; aFirst < gLast; --gLast)
			{
				if (aPredicate(myContainerToSort[gLast - 1], myContainerToSort[first]))
				{

				}
				else if (aPredicate(myContainerToSort[first], myContainerToSort[gLast - 1]))//remember we partition [aFirst,aLast).
				{
					break;
				}
				else if ((--first) != (gLast - 1)) //pre-order decrement, no clue why but I hate it.
				{
					myContainerToSort.Swap(first, gLast - 1);
				}
			}

			//our upper iterator has traveresed down to our lower bound and
			//our lower iterator has traveresed up to upper bound.
			if (gLast == aFirst && gFirst == aLast)
			{
				return std::pair<size_t, size_t>(first, last);//return our bounds as iterator pair.
			}

			if (gLast == aFirst)
			{
				if (last != gFirst)
				{
					myContainerToSort.Swap(first, last);
				}

				++last;
				myContainerToSort.Swap(first, gFirst);
				++first;
				++gFirst;
			}
			else if (gFirst == aLast)
			{
				if ((--gLast) != (--first))
				{
					myContainerToSort.Swap(gLast, first);
				}
				myContainerToSort.Swap(first, --last);
			}
			else
			{
				myContainerToSort.Swap(gFirst, --gLast);
				++gFirst;
			}
		}
	}

	template<class ... TypeList>
	inline void ISort<TypeList...>::GuessMedian(size_t aFirst, size_t aMid, size_t aLast, const PredicateSignature& aPredicate)
	{
		size_t count = aLast - aFirst;
		//Kind of arbitrarily picks a set of elements to move around for good pivots.
		if (40 < count)
		{
			size_t step = (count + 1) >> 3;//right shift 3 is a division by 8.(2^3)
			size_t twoStep = step << 1;// left shift is step * 2. unreadable but optimized?
			MedianOfThree(aFirst, aFirst + step, aFirst + twoStep, aPredicate);
			MedianOfThree(aMid - step, aMid, aMid + step, aPredicate);
			MedianOfThree(aLast - twoStep, aLast - step, aLast, aPredicate);
			MedianOfThree(aFirst + step, aMid, aLast - step, aPredicate);
		}
		else
		{
			MedianOfThree(aFirst, aMid, aLast, aPredicate);
		}
	}

	template<class ... TypeList>
	inline void ISort<TypeList...>::MedianOfThree(size_t aFirst, size_t aMid, size_t aLast, const PredicateSignature& aPredicate)
	{
		if (aPredicate(myContainerToSort[aMid], myContainerToSort[aFirst]))
		{
			myContainerToSort.Swap(aMid, aFirst);
		}

		if (aPredicate(myContainerToSort[aLast], myContainerToSort[aMid]))
		{
			myContainerToSort.Swap(aLast, aMid);
			if (aPredicate(myContainerToSort[aMid], myContainerToSort[aFirst]))
			{
				myContainerToSort.Swap(aMid, aFirst);
			}
		}
	}

	template<class ... TypeList>
	inline void ISort<TypeList...>::HeapSort(size_t aFirst, size_t aLast, const PredicateSignature& aPredicate)
	{
		// Build heap (rearrange array) 
		for (size_t i = (aLast / 2) - 1; (i >= aFirst) && (i < myContainerToSort.Size()); i--)
		{
			BuildHeap(aLast, i, aPredicate);
		}

		// One by one extract an element from heap 
		for (size_t i = aLast - 1; (i >= aFirst) && (i < myContainerToSort.Size()); i--)
		{
			myContainerToSort.Swap(aFirst, i);
			BuildHeap(i, aFirst, aPredicate);
		}
	}

	template<class ... TypeList>
	inline void ISort<TypeList...>::BuildHeap(size_t aFirst, size_t aLast, const PredicateSignature& aPredicate)
	{
		size_t risingIndex = aFirst;
		size_t left = 2 * aFirst + 1;
		size_t right = 2 * aFirst + 2;

		if ((left < aLast) && aPredicate(myContainerToSort.Get(risingIndex), myContainerToSort.Get(left)))
		{
			risingIndex = left;
		}

		if ((right < aLast) && aPredicate(myContainerToSort.Get(risingIndex), myContainerToSort.Get(right)))
		{
			risingIndex = right;
		}

		if (risingIndex != aFirst)
		{
			myContainerToSort.Swap(aFirst, risingIndex);
			BuildHeap(aLast, risingIndex, aPredicate);
		}
	}

	template<class ... TypeList>
	inline void ISort<TypeList...>::InsertionSort(size_t aFirst, size_t aLast, const PredicateSignature& aPredicate)
	{
		size_t i, j;
		std::tuple<TypeList...> tempStore;
		for (i = aFirst + 1; i < aLast; ++i)
		{
			tempStore = myContainerToSort.Get(i);
			std::tuple<TypeList&...> comp = TupleUtil<TypeList...>::ConstructReference(tempStore);
			j = i - 1;
			while ((j < myContainerToSort.Size()) && (j >= aFirst) && aPredicate(comp, myContainerToSort.Get(j)))
			{
				myContainerToSort[j + 1] = myContainerToSort[j];
				--j;
			}
			myContainerToSort[j + 1] = tempStore;
		}
	}


	template<TypeIndexType PredicateIndex, class PredSigType, class ... TypeList>
	struct ISortLite
	{
	public:
		using PredicateType = typename TemplateUtility::ChooseType<PredicateIndex, TypeList...>;

		using PredicateSignature = PredSigType;
		using IteratorType = typename std::vector<PredicateType>::iterator;
		using IteratorDiffType = typename IteratorType::difference_type;

		ISortLite(SoAC<TypeList...>& aContainerToSort, size_t aFirst, size_t aLast, const PredicateSignature& aPredicate) : myContainerToSort(aContainerToSort)
		{
			/*TODO: Implement std introsort much like ISort functor. Key here is that there should be considerably less overhead as only one array in the SoA needs accessing during Predicate.
				only cache problem should be the need for container.Swap(a,b).  consider using iterators instead of indices for cache considerations during predicate operations. resolve index requirements by:
				Iterator - container.begin() = index;*/
			std::vector<PredicateType>& container = aContainerToSort.GetTypeArray<PredicateIndex>();
			myContainerHead = container.begin();

			IteratorType iteratorBegin = myContainerHead + aFirst;
			IteratorType iteratorEnd = myContainerHead + aLast;
			IteratorDiffType ideal = iteratorEnd - iteratorBegin;
			IntroSort(iteratorBegin, iteratorEnd, ideal, aPredicate);
		}

	private:

		//inline IteratorType GetBegin() { return myContainerHead; }

		//inline void IntroSort(IteratorType aFirst, IteratorType aLast, size_t anIdeal, const PredicateSignature& aPredicate);
		//inline std::pair<IteratorType, IteratorType> PartitionByMedianGuess(IteratorType aFirst, IteratorType aLast, const PredicateSignature& aPredicate);
		//inline void GuessMedian(IteratorType aFirst, IteratorType aMid, IteratorType aLast, const PredicateSignature& aPredicate);
		//inline void MedianOfThree(IteratorType aFirst, IteratorType aMid, IteratorType aLast, const PredicateSignature& aPredicate);
		//inline void HeapSort(IteratorType aFirst, IteratorType aLast, const PredicateSignature& aPredicate);
		//inline void BuildHeap(IteratorType aFirst, IteratorType aLast, const PredicateSignature& aPredicate);
		//inline void InsertionSort(IteratorType aFirst, IteratorType aLast, const PredicateSignature& aPredicate);

		SoAC<TypeList...>& myContainerToSort;
		IteratorType myContainerHead;

		inline void IntroSort(IteratorType aFirst, IteratorType aLast, IteratorDiffType anIdeal, const PredicateSignature & aPredicate)
		{
			IteratorDiffType count;
			while ((ISORT_MAX < (count = aLast - aFirst)) && (0 < anIdeal))
			{
				std::pair<IteratorType, IteratorType> midBounds = PartitionByMedianGuess(aFirst, aLast, aPredicate);
				anIdeal = (anIdeal >> 1) + (anIdeal >> 2);

				if (midBounds.first - aFirst < aLast - midBounds.second)
				{
					IntroSort(aFirst, midBounds.first, anIdeal, aPredicate);
					aFirst = midBounds.second;
				}
				else
				{
					IntroSort(midBounds.second, aLast, anIdeal, aPredicate);
					aLast = midBounds.first;
				}
			}

			if (ISORT_MAX < count)
			{
				HeapSort(aFirst, aLast, aPredicate);
			}
			else if (2 <= count)
			{
				InsertionSort(aFirst, aLast, aPredicate);
			}
		}

		inline std::pair<IteratorType, IteratorType> PartitionByMedianGuess(IteratorType aFirst, IteratorType aLast, const PredicateSignature & aPredicate)
		{
			IteratorType mid = aFirst + ((aLast - aFirst) >> 1);
			GuessMedian(aFirst, mid, aLast - 1, aPredicate);
			IteratorType first = mid;
			IteratorType last = first + 1;

			while ((aFirst < first)
				&& (!aPredicate(*(first - 1), *first))
				&& (!aPredicate(*first, *(first - 1))))
			{
				--first;
			}

			while ((last < aLast)
				&& !aPredicate(*last, *first)
				&& !aPredicate(*first, *last))
			{
				++last;
			}

			IteratorType gFirst = last;
			IteratorType gLast = first;

			for (;;)
			{
				for (; gFirst < aLast; ++gFirst)
				{
					if (aPredicate(*first, *gFirst))
					{

					}
					else if (aPredicate(*gFirst, *first))
					{
						break;
					}
					else if (last != gFirst)
					{
						myContainerToSort.Swap(last - myContainerHead, gFirst - myContainerHead);
						++last;
					}
					else
					{
						++last;
					}
				}

				for (; aFirst < gLast; --gLast)
				{
					if (aPredicate(*(gLast - 1), *first))
					{

					}
					else if (aPredicate(*first, *(gLast - 1)))
					{
						break;
					}
					else if ((--first) != (gLast - 1))
					{
						myContainerToSort.Swap(first - myContainerHead, (gLast - 1) - myContainerHead);
					}
				}

				if (gLast == aFirst && gFirst == aLast)
				{
					return std::pair<IteratorType, IteratorType>(first, last);
				}

				if (gLast == aFirst)
				{
					if (last != gFirst)
					{
						myContainerToSort.Swap(first - myContainerHead, last - myContainerHead);
					}

					++last;
					myContainerToSort.Swap(first - myContainerHead, gFirst - myContainerHead);
					++first;
					++gFirst;
				}
				else if (gFirst == aLast)
				{
					if ((--gLast) != (--first))
					{
						myContainerToSort.Swap(gLast - myContainerHead, first - myContainerHead);
					}
					myContainerToSort.Swap(first - myContainerHead, (--last) - myContainerHead);
				}
				else
				{
					myContainerToSort.Swap(gFirst - myContainerHead, (--gLast) - myContainerHead);
					++gFirst;
				}
			}
		}

		inline void GuessMedian(IteratorType aFirst, IteratorType aMid, IteratorType aLast, const PredicateSignature & aPredicate)
		{
			size_t count = aLast - aFirst;
			if (40 < count)
			{
				size_t step = (count + 1) >> 3;
				size_t twoStep = step << 1;
				MedianOfThree(aFirst, aFirst + step, aFirst + twoStep, aPredicate);
				MedianOfThree(aMid - step, aMid, aMid + step, aPredicate);
				MedianOfThree(aLast - twoStep, aLast - step, aLast, aPredicate);
				MedianOfThree(aFirst + step, aMid, aLast - step, aPredicate);
			}
			else
			{
				MedianOfThree(aFirst, aMid, aLast, aPredicate);
			}
		}

		inline void MedianOfThree(IteratorType aFirst, IteratorType aMid, IteratorType aLast, const PredicateSignature & aPredicate)
		{
			if (aPredicate(*aMid, *aFirst))
			{
				myContainerToSort.Swap(aMid - myContainerHead, aFirst - myContainerHead);
			}

			if (aPredicate(*aLast, *aMid))
			{
				myContainerToSort.Swap(aLast - myContainerHead, aMid - myContainerHead);
				if (aPredicate(*aMid, *aFirst))
				{
					myContainerToSort.Swap(aMid - myContainerHead, aFirst - myContainerHead);
				}
			}
		}

		inline void HeapSort(IteratorType aFirst, IteratorType aLast, const PredicateSignature & aPredicate)
		{
			size_t firstIndex = aFirst - myContainerHead;
			size_t lastIndex = aLast - myContainerHead;

			for (size_t i = (lastIndex / 2) - 1; (i >= firstIndex) && (i < myContainerToSort.Size()); i--)
			{
				BuildHeap(myContainerHead + lastIndex, myContainerHead + i, aPredicate);
			}

			for (size_t i = lastIndex - 1; (i >= firstIndex) && (i < myContainerToSort.Size()); i--)
			{
				myContainerToSort.Swap(firstIndex, i);
				BuildHeap(myContainerHead + i, myContainerHead + firstIndex, aPredicate);
			}
		}

		inline void BuildHeap(IteratorType aFirst, IteratorType aLast, const PredicateSignature & aPredicate)
		{
			IteratorType rising = aFirst;
			size_t baseIndex = (aFirst - myContainerHead) * 2;
			IteratorType left = (myContainerHead + baseIndex) + 1;
			IteratorType right = (myContainerHead + baseIndex) + 2;

			if ((left < aLast) && aPredicate(*rising, *left))
			{
				rising = left;
			}

			if ((right < aLast) && aPredicate(*rising, *right))
			{
				rising = right;
			}

			if (rising != aFirst)
			{
				myContainerToSort.Swap(aFirst - myContainerHead, rising - myContainerHead);
				BuildHeap(aLast, rising, aPredicate);
			}
		}

		inline void InsertionSort(IteratorType aFirst, IteratorType aLast, const PredicateSignature & aPredicate)
		{
			IteratorType i, j;
			PredicateType predicateValue;
			std::tuple<TypeList...> t;
			for (i = aFirst + 1; i < aLast; ++i)
			{
				predicateValue = *i;
				j = i - 1;
				while ((j < aLast) && (j >= aFirst) && aPredicate(predicateValue, *j))
				{
					myContainerToSort[(j + 1) - myContainerHead] = myContainerToSort[j - myContainerHead];
					--j;
				}
				myContainerToSort[(j + 1) - myContainerHead] = myContainerToSort[i - myContainerHead];
			}
		}

	};
}

namespace CU = CommonUtility;