#include "pch.h"
#include "Timer.h"

CommonUtility::Clock::Clock()
{
	myTotalTime = 0.0;
	myFrameTime = std::chrono::high_resolution_clock::now();
	myLastFrameTime = std::chrono::high_resolution_clock::now();
}

void CommonUtility::Clock::Update()
{
	myLastFrameTime = myFrameTime;
	myFrameTime = std::chrono::high_resolution_clock::now();
	myDt = myFrameTime - myLastFrameTime;
	myTotalTime += GetDeltaTime();
}

float CommonUtility::Clock::GetDeltaTime() const
{
	return myDt.count();
}

double CommonUtility::Clock::GetTotalTime() const
{
	return myTotalTime;
}