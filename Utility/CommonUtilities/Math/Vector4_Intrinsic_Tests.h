#pragma once
#include <immintrin.h>
#include "FunctionPointer.h"

class alignas(16) XVec4
{
public:
	XVec4() {}
	XVec4(float aX, float aY, float aZ, float aW) : x(aX), y(aY), z(aZ), w(aW) {}
	~XVec4() {}

	inline void operator+=(const XVec4& aRHS)
	{
		myIntrinsicRep = _mm_add_ps(myIntrinsicRep, aRHS.myIntrinsicRep);
		//x += aRHS.x;
		//y += aRHS.y;
		//z += aRHS.z;
		//w += aRHS.w;
	}

	inline XVec4 operator+(const XVec4& aRHS) const
	{
		return XVec4(x + aRHS.x, y + aRHS.y, z + aRHS.z, w + aRHS.w);
	}
#pragma warning(push)
#pragma warning(disable : 4201)
	union
	{
		struct {
			float x, y, z, w;
		};
		__m128 myIntrinsicRep;
	};
#pragma warning(pop)

private:
};

inline void XVec4Add(XVec4& aLHS, const XVec4& aRHS)
{
	aLHS.x += aRHS.x;
	aLHS.y += aRHS.y;
	aLHS.z += aRHS.z;
	aLHS.w += aRHS.w;
}

inline void XVec4Add_SSE(XVec4& aLHS, const XVec4& aRHS)
{
	aLHS.myIntrinsicRep = _mm_add_ps(aLHS.myIntrinsicRep, aRHS.myIntrinsicRep);
}

class XVec4Math
{
public:
	virtual inline void Add(XVec4& aLHS, const XVec4& aRHS)
	{
		XVec4Add(aLHS, aRHS);
	}
};

class XVec4Math_SSE : public XVec4Math
{
public:
	virtual inline void Add(XVec4& aLHS, const XVec4& aRHS) override
	{
		XVec4Add_SSE(aLHS, aRHS);
	}
};

class IntrinsicMathematics
{
public:

	enum class IntrinsicCPUType
	{	
		SSE2,
		SSE42,
		SSE,
		Regular,
		AVX,
		AVX512
	};

	static inline void Init()
	{
		switch (ourChosenCpuID)
		{
		case IntrinsicCPUType::Regular:
			ourMathSolver = new XVec4Math();
			ourAddFunction = ExplicitFunction<void, XVec4&, const XVec4&>(&XVec4Add);
			break;
		case IntrinsicCPUType::AVX512:
		case IntrinsicCPUType::AVX:
		case IntrinsicCPUType::SSE42:
		case IntrinsicCPUType::SSE2:
		case IntrinsicCPUType::SSE:
			ourMathSolver = new XVec4Math_SSE();
			ourAddFunction = ExplicitFunction<void, XVec4&, const XVec4&>(&XVec4Add_SSE);
			break;
		default:
			break;
		}
	}

	static inline void AddViaVTable(XVec4& aLHS, const XVec4& aRHS)
	{
		ourMathSolver->Add(aLHS, aRHS);
	}

	static inline void AddViaSwitchCase(XVec4& aLHS, const XVec4& aRHS)
	{
		switch (ourChosenCpuID)
		{
		case IntrinsicCPUType::Regular:
			XVec4Add(aLHS, aRHS);
			break;
		case IntrinsicCPUType::AVX512:
		case IntrinsicCPUType::AVX:
		case IntrinsicCPUType::SSE42:
		case IntrinsicCPUType::SSE2:
		case IntrinsicCPUType::SSE:
			XVec4Add_SSE(aLHS, aRHS);
			break;
		default:
			break;
		}
	}

	static inline void AddViaFuncPtr(XVec4& aLHS, const XVec4& aRHS)
	{
		ourAddFunction(aLHS, aRHS);
	}

private:
	inline static IntrinsicCPUType ourChosenCpuID = IntrinsicCPUType::SSE;
	inline static XVec4Math* ourMathSolver = nullptr;
	inline static ExplicitFunction<void, XVec4&, const XVec4&> ourAddFunction;
};