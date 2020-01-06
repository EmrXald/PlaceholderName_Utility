#pragma once
#include "Math/Vector4_Intrinsic_Tests.h"
#include "StopWatch.h"
#include <vector>
#include <iostream>
#include "Container/SoAC.h"
#include "Container/SoACUtilities.h"
#include "Math/CommonMath.h"
#include "TemplateUtility/TypeInformation.h"

namespace IntrinsicMathTest
{
	inline void VectorAddTest()
	{
		IntrinsicMathematics::Init();
		const int simulations = 10;
		const int iterations = 100000;

		struct ResultSave
		{
			long long defaultTime = 0;
			long long vTableTime = 0;
			long long funcPtrTime = 0;
			long long switchCaseTime = 0;
		};
		std::vector<ResultSave> simulationResults;

		for (int s = 0; s < simulations; ++s)
		{
			CU::StopWatch vecTimer;
			ResultSave newResult;
			{//Default operation.
				XVec4 xVecA(0.0f, 0.0f, 0.0f, 0.0f), xVecB(1.0f, 1.0f, 1.0f, 1.0f);
				vecTimer.Start();
				for (int i = 0; i < iterations; ++i)
				{
					xVecA += xVecB;
				}
				vecTimer.Stop();
				newResult.defaultTime = vecTimer.Time().count();
				std::cout << xVecA.x << xVecA.y << xVecA.z << std::endl;
			}
			{//V-Table Operation
				XVec4 xVecA(0.0f, 0.0f, 0.0f, 0.0f), xVecB(1.0f, 1.0f, 1.0f, 1.0f);
				vecTimer.Start();
				for (int i = 0; i < iterations; ++i)
				{
					IntrinsicMathematics::AddViaVTable(xVecA, xVecB);
				}
				vecTimer.Stop();
				newResult.vTableTime = vecTimer.Time().count();
				std::cout << xVecA.x << xVecA.y << xVecA.z << std::endl;
			}
			{//Func ptr operation.
				XVec4 xVecA(0.0f, 0.0f, 0.0f, 0.0f), xVecB(1.0f, 1.0f, 1.0f, 1.0f);
				vecTimer.Start();
				for (int i = 0; i < iterations; ++i)
				{
					IntrinsicMathematics::AddViaFuncPtr(xVecA, xVecB);
				}
				vecTimer.Stop();
				newResult.funcPtrTime = vecTimer.Time().count();
				std::cout << xVecA.x << xVecA.y << xVecA.z << std::endl;
			}
			{//switch case operation.
				XVec4 xVecA(0.0f, 0.0f, 0.0f, 0.0f), xVecB(1.0f, 1.0f, 1.0f, 1.0f);
				vecTimer.Start();
				for (int i = 0; i < iterations; ++i)
				{
					IntrinsicMathematics::AddViaSwitchCase(xVecA, xVecB);
				}
				vecTimer.Stop();
				newResult.switchCaseTime = vecTimer.Time().count();
				std::cout << xVecA.x << xVecA.y << xVecA.z << std::endl;
			}
			simulationResults.push_back(newResult);
		}
		std::cout << "D:\n";
		for (const ResultSave& c : simulationResults)
		{
			std::cout << c.defaultTime << std::endl;
		}
		std::cout << "V:\n";
		for (const ResultSave& c : simulationResults)
		{
			std::cout << c.vTableTime << std::endl;
		}
		std::cout << "F:\n";
		for (const ResultSave& c : simulationResults)
		{
			std::cout << c.funcPtrTime << std::endl;
		}
		std::cout << "S:\n";
		for (const ResultSave& c : simulationResults)
		{
			std::cout << c.switchCaseTime << std::endl;
		}
	}
}

struct IntFloat
{
	IntFloat() {}
	IntFloat(const int aI, const float aF) : i(aI), f(aF) {}
	int i;
	float f;
	inline const bool operator<(const IntFloat& aRHS) const
	{
		return i < aRHS.i;
	}

	inline const bool operator>(const IntFloat& aRHS) const
	{
		return i > aRHS.i;
	}

	inline const bool operator<=(const IntFloat& aRHS) const
	{
		return i <= aRHS.i;
	}
};

void ValidateSort(const std::vector<IntFloat>& aSortedVector)
{
	for (size_t i = 0; i < aSortedVector.size() - 1; ++i)
	{
		const IntFloat& outer = aSortedVector[i];
		for (size_t j = i + 1; j < aSortedVector.size(); ++j)
		{
			const IntFloat& inner = aSortedVector[j];
			assert(outer <= inner && "Vector is not sorted!");
			outer, inner;
		}
	}
}

template<typename ... TypeList>
void ValidateSort(const CU::SoAC<TypeList...>& aSortedVector, const typename CU::ISort< TypeList...>::PredicateSignature& aPredicate)
{
	for (size_t i = 0; i < aSortedVector.Size() - 1; ++i)
	{
		const std::tuple<const TypeList&...> outer = aSortedVector[i];
		for (size_t j = i + 1; j < aSortedVector.Size(); ++j)
		{
			const std::tuple<const TypeList&...> inner = aSortedVector[j];
			bool predicatePassed = aPredicate(outer, inner);
			bool inversePredicatePassed = aPredicate(inner, outer);
			bool wereEqual = (!predicatePassed) && (!inversePredicatePassed);
			assert((predicatePassed || wereEqual) && "Vector is not sorted!");
			outer, inner, wereEqual;
		}
	}
}

template<TypeIndexType PredicateIndex, class PredSigType, typename ... TypeList>
void ValidateSort(const CU::SoAC<TypeList...>& aSortedVector, const PredSigType& aPredicate)
{
	using PredicateType = TemplateUtility::ChooseType<PredicateIndex, TypeList...>;
	for (size_t i = 0; i < aSortedVector.Size() - 1; ++i)
	{
		const PredicateType& outer = aSortedVector.Get<PredicateIndex>(i);
		for (size_t j = i + 1; j < aSortedVector.Size(); ++j)
		{
			const PredicateType& inner = aSortedVector.Get<PredicateIndex>(j);
			bool predicatePassed = aPredicate(outer, inner);
			bool inversePredicatePassed = aPredicate(inner, outer);
			bool wereEqual = (!predicatePassed) && (!inversePredicatePassed);
			assert((predicatePassed || wereEqual) && "Vector is not sorted!");
			outer, inner, wereEqual;
		}
	}
}

namespace SoACSortTests
{
	inline void SoACSortSpeedTest()
	{
		CU::SoAC<int, float> soacA;
		std::vector<IntFloat> vecQuick;
		std::vector<IntFloat> vecSTD;
		std::vector<IntFloat> vecIntro;
		for (int i = 0; i < 100'000; ++i)
		{
			float f = CU::GenerateRandomRealNumber<float>(-10.f,10.f);
			int ii = CU::GenerateRandomInteger(-10, 10);
			soacA.Add(ii, f);
			vecQuick.push_back(IntFloat(ii, f));
			vecSTD.push_back(IntFloat(ii, f));
			vecIntro.push_back(IntFloat(ii, f));
		}
		
		CU::ISort<int, float>::PredicateSignature predicate = [&](const auto& aLHS, const auto& aRHS) {return std::get<0>(aLHS) < std::get<0>(aRHS); };
		auto predicateLite = [](const int aLHS, const int aRHS) {return aLHS < aRHS; };
		CU::StopWatch s;
		
		s.Start();
		CU::ISortLite<0, decltype(predicateLite), int, float>(soacA, 0, soacA.Size(), predicateLite);
		s.Stop();
		
		ValidateSort<0>(soacA, predicateLite);
		auto soACTime = s.Time().count();
		std::cout << "SoAC intro sort: " << soACTime << "\n";
		
		s.Start();
		std::sort(vecSTD.begin(), vecSTD.end(), [](const IntFloat& aLHS, const IntFloat& aRHS) {return aLHS < aRHS; });
		s.Stop();
		ValidateSort(vecSTD);
		auto stdTime = s.Time().count();
		
		std::cout << "std quicksort: " << stdTime << "\n";
		
		const double percentDiff2 =  (double)soACTime / (double)stdTime;
		std::cout << " SoAC / std::sort : " << percentDiff2 << ".\n";
		
	}
}

class CommonBase
{
public:

	template<class T>
	inline bool IsType() const
	{
		return IsTypeInternal(RTTI::GetRuntimeTypeID<T>());
	}

protected:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const
	{
		return  TU::GetTypeID<CommonBase>() == aTypeID;
	}

};

template<class T>
class MetaBase : public CommonBase
{
public:

protected:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const
	{
		return  TU::GetTypeID<T>() == aTypeID || CommonBase::IsTypeInternal(aTypeID);
	}

};

template<class T, class BaseClass>
class MetaExtension : public BaseClass
{
public:

protected:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const
	{
		return  TU::GetTypeID<T>() == aTypeID || BaseClass::IsTypeInternal(aTypeID);
	}

};

class Derived : public MetaBase<Derived>
{
public:

private:

};

class MetaDerived2 : public MetaExtension<MetaDerived2, Derived>
{
public:

private:

};

class MetaDerived3 : public MetaExtension<MetaDerived3, MetaDerived2>
{
public:

private:

};

class MetaDerived4 : public MetaExtension<MetaDerived4, MetaDerived3>
{
public:

private:

};

class MetaDerived5 : public MetaExtension<MetaDerived5, MetaDerived4>
{
public:

private:

};

class MetaDerived6 : public MetaExtension<MetaDerived6, MetaDerived5>
{
public:

private:

};

class MetaDerived7 : public MetaExtension<MetaDerived7, MetaDerived6>
{
public:

private:

};

class ExplicitDerived : public CommonBase
{

public:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const override
	{
		return TU::GetTypeID<ExplicitDerived>() == aTypeID || CommonBase::IsTypeInternal(aTypeID);
	}

private:
};

class ExplicitDerived2 : public ExplicitDerived
{
public:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const override
	{
		return TU::GetTypeID<ExplicitDerived2>() == aTypeID || ExplicitDerived::IsTypeInternal(aTypeID);
	}

private:
};

class ExplicitDerived3 : public ExplicitDerived2
{
public:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const override
	{
		return TU::GetTypeID<ExplicitDerived3>() == aTypeID || ExplicitDerived2::IsTypeInternal(aTypeID);
	}

private:
};

class ExplicitDerived4 : public ExplicitDerived3
{
public:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const override
	{
		return TU::GetTypeID<ExplicitDerived4>() == aTypeID || ExplicitDerived3::IsTypeInternal(aTypeID);
	}

private:
};

class ExplicitDerived5 : public ExplicitDerived4
{
public:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const override
	{
		return TU::GetTypeID<ExplicitDerived5>() == aTypeID || ExplicitDerived4::IsTypeInternal(aTypeID);
	}

private:
};

class ExplicitDerived6 : public ExplicitDerived5
{
public:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const override
	{
		return TU::GetTypeID<ExplicitDerived6>() == aTypeID || ExplicitDerived5::IsTypeInternal(aTypeID);
	}

private:
};

class ExplicitDerived7 : public ExplicitDerived6
{
public:

	inline virtual bool IsTypeInternal(const TU::TypeID aTypeID) const override
	{
		return TU::GetTypeID<ExplicitDerived7>() == aTypeID || ExplicitDerived6::IsTypeInternal(aTypeID);
	}

private:
};

namespace TypeInformationTests
{
	//Tests that IsType returns the proper results.
	inline void FeatureTest()
	{
		MetaDerived3 d3;
		CommonBase* asBase = &d3;
		const bool IsCommonBase = asBase->IsType<CommonBase>();
		const bool IsDerived = asBase->IsType<Derived>();
		const bool IsMetaDerived2 = asBase->IsType<MetaDerived2>();
		const bool IsMetaDerived3 = asBase->IsType<MetaDerived3>();
		const bool IsNotInt = asBase->IsType<int>();
		IsCommonBase;
		IsDerived;
		IsMetaDerived2;
		IsMetaDerived3;
		IsNotInt;
	}

	//Does a large number of IsType() via template generated IsType().
	template<class TypeToCheck, class TrueTypeToCheckAgainst, class FalseTypeToCheckAgainst>
	inline void LargeVolumeTypeComparsionTest1()
	{
		TypeToCheck a;
		CommonBase* asBase = &a;
		int totalTrueComparisons = 0;
		int totalFalseComparisons = 0;
		for (int i = 0; i < 1000000 ; ++i)
		{
			if (i % 2 == 0)
				totalTrueComparisons += asBase->IsType<TrueTypeToCheckAgainst>() ? 1 : 0;
			else
				totalFalseComparisons += !asBase->IsType<FalseTypeToCheckAgainst>() ? 1 : 0;
		}
		
		std::cout << totalTrueComparisons << " : " << totalFalseComparisons << std::endl;
	}

	//Does a large number of IsType() via explicitly written IsType().
	inline void LargeVolumeTypeComparsionTest2()
	{

	}

	template<class T>
	inline void PrintCTTIStringAndID()
	{
		std::cout << CTTI::Cexpr_TypeName<T>() << " : " << CTTI::Cexpr_TypeID<T>() << std::endl;
	}

	template<class T>
	inline void PrintRTTIStringAndID()
	{
		TypeIDHandle<T> typeHandle;
		const char* typeName = typeHandle.GetTypeName();
		const auto id = typeHandle.GetTypeID();

		std::cout << typeName << " : " << id << std::endl;
	}
}

using NetworkHandshakeMessage = NetworkMessageGeneric<std::string>;
using NetworkConfirmMessage = NetworkMessageGeneric<>;

namespace NetworkMessagingTests
{
	//Serialises a network message, hands it to the terminal for deserialisation. Terminal deserialises it and is then printed to the console.
	inline void TestTerminalSerialisationAndDeserialisation()
	{
		NetworkMessageTerminal nmt;
		nmt.RegisterTypes<NetworkHandshakeMessage, NetworkConfirmMessage>();
		
		NetworkConfirmMessage confirm;
		NetworkHandshakeMessage handshake;
		handshake.SetData<0>(std::string("what"));
		nmt.PackMessage<NetworkHandshakeMessage>(handshake,1,2);
		nmt.PackMessage<NetworkConfirmMessage>(confirm, 2,1);
		nmt.StoreMessage(handshake.GetBinaryData());
		
		const std::vector<NetworkHandshakeMessage>& confMsg = nmt.GetMessages<NetworkHandshakeMessage>();
		
		std::cout << "Hello World!\n" << confMsg[0].GetData<0>() << "\n";
	}
}