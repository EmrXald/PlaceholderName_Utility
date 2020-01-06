#pragma once

template<class ReturnType, class ... InParameterTypes>
class ExplicitFunction
{
public:
	ExplicitFunction() : myFunction(nullptr) {}
	ExplicitFunction(ReturnType(*aFunc)(InParameterTypes...)) : myFunction(aFunc) {}
	~ExplicitFunction() {}

	inline ReturnType operator()(InParameterTypes ... someInParameters)
	{
		return myFunction(someInParameters...);
	}

private:
	ReturnType(*myFunction)(InParameterTypes...);
};

template<class ClassType, class ReturnType, class ... InParameterTypes>
class ExplicitMethod
{
public:
	ExplicitMethod() : myFunction(nullptr) {}
	ExplicitMethod(ReturnType(ClassType::*aFunc)(InParameterTypes...)) : myFunction(aFunc) {}
	~ExplicitMethod() {}

	inline ReturnType operator()(ClassType& anInstanceToInvokeOn, InParameterTypes ... someInParameters)
	{
		return (anInstanceToInvokeOn.*myFunction)(someInParameters ...);
	}

private:
	ReturnType(ClassType::*myFunction)(InParameterTypes...);
};
