// CommonUtilities.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Entity Component System/EntityRegistry.h"
#include "BinarySerialiser.h"
#include "Network/NetworkMessageTerminal.h"
#include "Network/NetworkMessageGeneric.h"
#include "Math/CommonMath.h"
#include "Container/SoAC.h"
#include "StopWatch.h"
#include "Container/IntroSort.h"
#include "Container/SoACUtilities.h"
#include "Container/SparseVector.h"
#include "Container/UniqueTypeMap.h"
#include "FeatureTests.h"

struct NetA : public NetworkMessageBase {};
struct NetB : public NetworkMessageBase {};
struct NetC : public NetworkMessageBase {};

struct A {};
struct B {};
struct C {};
enum class DummyFamily;

class RValueTest
{
public:
	RValueTest() { myInt = 0; }
	RValueTest(const int aVal) : myInt(aVal) {}
	RValueTest(const RValueTest& aRHS)
	{
		*this = aRHS;
	}

	RValueTest(RValueTest&& aRHS) : RValueTest()
	{
		*this = std::forward<RValueTest>(aRHS);
	}

	RValueTest& operator=(const RValueTest& aRHS)
	{
		std::cout << "Regular assignment overload reached.\n";
		myInt = aRHS.myInt;
		myInt--;
		return *this;
	}

	RValueTest& operator=(RValueTest&& aRHS)
	{
		std::cout << "Move semantic overload reached.\n";
		std::swap(myInt, aRHS.myInt);
		myInt++;
		return *this;
	}

private:
	int myInt;
};

void AddSV(CU::SparseVector<IntFloat>& a, const unsigned int e, const int i, const float f)
{
	IntFloat& c = a.Add(e);
	c.i = i;
	c.f = f;
}

void RemoveCyclicSV(CU::SparseVector<IntFloat>& a, const unsigned int e)
{
	a.RemoveCyclic(e);
}

void RemoveSV(CU::SparseVector<IntFloat>& a, const unsigned int e)
{
	a.Remove(e);
}

int main()
{
	EntityRegistry eReg;
	const Entity e1 = eReg.Create();
	const Entity e2 = eReg.Create();
	const Entity e3 = eReg.Create();

	const Entity e4 = eReg.Create();
	const Entity e5 = eReg.Create();
	const Entity e6 = eReg.Create();

	eReg.Assign<float>(e4);
	eReg.Assign<char>(e5);
	eReg.Assign<char>(e6);

	int& newIntComponent1 = eReg.Assign<int>(e1);
	newIntComponent1 = 1;
	int& newIntComponent2 = eReg.Assign<int>(e2);
	newIntComponent2 = 2;
	int& newIntComponent3 = eReg.Assign<int>(e3);
	newIntComponent3 = 3;
	
	float& newfloatComponent1 = eReg.Assign<float>(e1);
	newfloatComponent1 = 1.0f;
	float& newfloatComponent2 = eReg.Assign<float>(e2);
	newfloatComponent2 = 2.0f;
	
	char& newBoolComponent1 = eReg.Assign<char>(e1);
	newBoolComponent1 = '1';

	CU::SparseSet<Entity> intEntities;
	eReg.Collection<int>(intEntities);
	
	CU::SparseSet<Entity> intFloatEntities;
	eReg.Collection<int, float>(intFloatEntities);

	CU::SparseSet<Entity> intFloatCharEntities;
	eReg.Collection<int, float, char>(intFloatCharEntities);

	eReg.Remove<int>(e1);
	eReg.Destroy(e1);

	CU::StopWatch s;
	for (int i = 0; i < 10; ++i)
	{
		s.Start();
		TypeInformationTests::LargeVolumeTypeComparsionTest1<MetaDerived6, CommonBase, MetaDerived7>();
		s.Stop();
		std::cout << "time: " << s.Time().count() << "\n";
	}
}