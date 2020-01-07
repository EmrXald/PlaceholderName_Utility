#pragma once

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

#include <chrono>

namespace CommonUtility
{
	class Clock
	{
	public:

		DLL_API Clock();
		DLL_API Clock(const Clock &aTimer) = delete;
		DLL_API Clock& operator=(const Clock &aTimer) = delete;

		DLL_API void Update();

		DLL_API float GetDeltaTime() const;
		DLL_API double GetTotalTime() const;

	private:

		std::chrono::high_resolution_clock::time_point myLastFrameTime;
		std::chrono::high_resolution_clock::time_point myFrameTime;
		std::chrono::duration<float> myDt;
		double myTotalTime;

	};
}

namespace CU = CommonUtility;