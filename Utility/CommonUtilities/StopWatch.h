#pragma once
#include <chrono>

namespace CommonUtility
{
	class StopWatch
	{
	public:
		StopWatch() {}
		~StopWatch() {}

		void Start()
		{
			myStart = std::chrono::high_resolution_clock::now();
		}

		void Stop()
		{
			myEnd = std::chrono::high_resolution_clock::now();
		}

		auto Time()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(myEnd - myStart);
		}

		template<class NumericType>
		auto TimeAs()
		{
			std::chrono::duration<NumericType> dt = myEnd - myStart;
			return dt.count();
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> myStart;
		std::chrono::time_point<std::chrono::steady_clock> myEnd;
	};
}

namespace CU = CommonUtility;