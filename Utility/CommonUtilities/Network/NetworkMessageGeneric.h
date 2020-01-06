#pragma once
#include "NetworkMessageBase.h"

template<class ... Types>
class NetworkMessageGeneric : public NetworkMessageBase
{
public:
	template<size_t TupleTypeIndex>
	using TupleType = TemplateUtility::ChooseType<TupleTypeIndex, Types...>;

	NetworkMessageGeneric() {}
	~NetworkMessageGeneric() {}

	template<size_t TupleTypeIndex>
	void SetData(const TupleType<TupleTypeIndex>& someData);

	template<size_t TupleTypeIndex>
	const TupleType<TupleTypeIndex>& GetData() const;

private:

	void SerialiseInternal(std::vector<char>& someBinaryData, size_t& aDataIterator) override;
	void DeserialiseInternal(const std::vector<char>& someBinaryData, size_t& aDataIterator) override;

	std::tuple<Types...> myGenericData;
};

template<class...Types>
inline void NetworkMessageGeneric<Types...>::SerialiseInternal(std::vector<char>& someBinaryData, size_t & aDataIterator)
{
	CU::BinarySerialise<decltype(myGenericData)>(someBinaryData, aDataIterator, myGenericData);
}

template<class...Types>
inline void NetworkMessageGeneric<Types...>::DeserialiseInternal(const std::vector<char>& someBinaryData, size_t & aDataIterator)
{
	CU::BinaryDeserialise<decltype(myGenericData)>(someBinaryData, aDataIterator, myGenericData);
}

template<class ...Types>
template<size_t TupleTypeIndex>
inline void NetworkMessageGeneric<Types...>::SetData(const TupleType<TupleTypeIndex>& someData)
{
	std::get<TupleTypeIndex>(myGenericData) = someData;
}

template<class ...Types>
template<size_t TupleTypeIndex>
inline const NetworkMessageGeneric<Types...>::TupleType<TupleTypeIndex>& NetworkMessageGeneric<Types...>::GetData() const
{
	return std::get<TupleTypeIndex>(myGenericData);
}

template<>
class NetworkMessageGeneric<> : public NetworkMessageBase
{
public:

	NetworkMessageGeneric() {}
	~NetworkMessageGeneric() {}

private:

	void SerialiseInternal(std::vector<char>& someBinaryData, size_t& aDataIterator) override;
	void DeserialiseInternal(const std::vector<char>& someBinaryData, size_t& aDataIterator) override;

	std::tuple<> myGenericData;
};

inline void NetworkMessageGeneric<>::SerialiseInternal(std::vector<char>& someBinaryData, size_t & aDataIterator)
{
	someBinaryData; aDataIterator;
}

inline void NetworkMessageGeneric<>::DeserialiseInternal(const std::vector<char>& someBinaryData, size_t & aDataIterator)
{
	someBinaryData; aDataIterator;
}
