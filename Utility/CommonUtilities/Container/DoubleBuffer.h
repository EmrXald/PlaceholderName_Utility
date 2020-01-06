#pragma once
#include <vector>

namespace CommonUtility
{
	template<class Type>
	class DoubleBuffer
	{
	public:
		DoubleBuffer() {}
		DoubleBuffer(const size_t& aReserveSize)
		{
			myBuffers[0].reserve(aReserveSize);
			myBuffers[1].reserve(aReserveSize);
			myReadIndex = 0;
			myWriteIndex = 1;
		}

		~DoubleBuffer() {}

		const std::vector<Type>& GetRead() const;
		std::vector<Type>& GetWrite();

		void Switch();
		void SwitchClear();

	private:

		std::vector<Type> myBuffers[2];

		size_t myReadIndex;
		size_t myWriteIndex;

	};

	template<class Type>
	inline const std::vector<Type>& DoubleBuffer<Type>::GetRead() const
	{
		return myBuffers[myReadIndex];
	}

	template<class Type>
	inline std::vector<Type>& DoubleBuffer<Type>::GetWrite()
	{
		return myBuffers[myWriteIndex];
	}

	template<class Type>
	inline void DoubleBuffer<Type>::Switch()
	{
		myReadIndex++;
		myWriteIndex = myReadIndex + 1;

		myReadIndex %= 2;
		myWriteIndex %= 2;
	}

	template<class Type>
	inline void DoubleBuffer<Type>::SwitchClear()
	{
		Switch();
		GetWrite().clear();
	}
}
namespace CU = CommonUtility;