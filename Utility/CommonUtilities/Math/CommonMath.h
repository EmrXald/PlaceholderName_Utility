#pragma once
#include <random>
#define MATH_PI 3.14159265359f

namespace CommonUtility
{
	template<class T>
	inline T GenerateRandomRealNumber(const T aMin, const T aMax)
	{
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::uniform_real_distribution<T> dist(static_cast<T>(aMin), static_cast<T>(aMax));

		return static_cast<T>(dist(mt));
	}

	template<class T>
	inline int GenerateRandomInteger(const T aMin, const T aMax)
	{
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::uniform_int_distribution<T> dist(aMin, aMax);

		return static_cast<int>(dist(mt));
	}

	template<class T>
	inline const T Lerp(const T& aStartValue, const T& anEndValue, const float anAlpha)
	{
		return aStartValue + anAlpha * (anEndValue - aStartValue);
	}

	inline float DegToRadians(const float anAngleInDegrees)
	{
		return anAngleInDegrees * (MATH_PI / 180.0f);
	}

	inline float RadToDegrees(const float anAngleInRadians)
	{
		return anAngleInRadians * (180.0f / MATH_PI);
	}
}
namespace CU = CommonUtility;
