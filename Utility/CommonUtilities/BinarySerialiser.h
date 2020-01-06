#pragma once
#include <string>
#include <vector>
#include "TemplateUtility/ChooseType.h"

namespace CommonUtility
{
	template<class T>
	struct BinarySerialise
	{
		BinarySerialise(std::vector<char>& someBinaryData, size_t& aDataIterator, const T& someData)
		{
			const size_t dataSize = sizeof(someData);
			const size_t endIndex = aDataIterator + dataSize;

			if (endIndex > someBinaryData.size())
			{
				someBinaryData.resize(endIndex + 1);
			}

			memcpy(&(someBinaryData[aDataIterator]), &someData, dataSize);
			aDataIterator = endIndex;
		}
	};

	template<class T>
	struct BinarySerialise<std::vector<T>>
	{
		BinarySerialise(std::vector<char>& someBinaryData, size_t& aDataIterator, const std::vector<T>& someData)
		{
			BinarySerialise<size_t>(someBinaryData, aDataIterator, someData.size());

			const size_t dataSize = sizeof(T) * someData.size();
			const size_t endIndex = aDataIterator + dataSize;

			if (endIndex > someBinaryData.size())
			{
				someBinaryData.resize(endIndex + 1);
			}

			memcpy(&(someBinaryData[aDataIterator]), &(someData[0]), dataSize);
			aDataIterator = endIndex;
		}
	};

	template<>
	struct BinarySerialise<std::string>
	{
		BinarySerialise(std::vector<char>& someBinaryData, size_t& aDataIterator, const std::string& someData)
		{
			BinarySerialise<size_t>(someBinaryData, aDataIterator, someData.length());

			const size_t dataSize = sizeof(std::string::value_type) * someData.length();
			const size_t endIndex = aDataIterator + dataSize;

			if (endIndex > someBinaryData.size())
			{
				someBinaryData.resize(endIndex + 1);
			}

			memcpy(&(someBinaryData[aDataIterator]), someData.c_str(), dataSize);
			aDataIterator = endIndex;
		}
	};

	template<class ... T>
	struct BinarySerialise<std::tuple<T...>>
	{
		BinarySerialise(std::vector<char>& someBinaryData, size_t& aDataIterator, const std::tuple<T...>& someData)
		{
			TupleSerialise(someBinaryData, aDataIterator, someData, std::make_index_sequence<sizeof...(T)>{});
		}
	private:

		template<size_t ... IndexSequence>
		inline void TupleSerialise(std::vector<char>& someBinaryData, size_t& aDataIterator, const std::tuple<T...>& someData, const std::index_sequence<IndexSequence...>&)
		{
			(BinarySerialise<T>(someBinaryData, aDataIterator, std::get<IndexSequence>(someData)), ...);
		}
	};

	template<class T>
	struct BinaryDeserialise
	{
		BinaryDeserialise(const std::vector<char>& someBinaryData, size_t& aDataIterator, T& someOutData)
		{
			memcpy(&someOutData, &(someBinaryData[aDataIterator]), sizeof(someOutData));
			aDataIterator += sizeof(someOutData);
		}
	};

	template<class T>
	struct BinaryDeserialise<std::vector<T>>
	{
		BinaryDeserialise(const std::vector<char>& someBinaryData, size_t& aDataIterator, std::vector<T>& someOutData)
		{
			size_t vectorSize = 0;
			BinaryDeserialise<size_t>(someBinaryData, aDataIterator, vectorSize);

			someOutData.resize(vectorSize);
			memcpy(&(someOutData[0]), &(someBinaryData[aDataIterator]), sizeof(T) * vectorSize);
			aDataIterator += sizeof(T) * vectorSize;
		}
	};

	template<>
	struct BinaryDeserialise<std::string>
	{
		BinaryDeserialise(const std::vector<char>& someBinaryData, size_t& aDataIterator, std::string& someOutData)
		{
			size_t stringLength = 0;
			BinaryDeserialise<size_t>(someBinaryData, aDataIterator, stringLength);

			someOutData.resize(stringLength);
			memcpy(&(someOutData[0]), &(someBinaryData[aDataIterator]), stringLength);
			aDataIterator += stringLength * sizeof(char);
		}
	};

	template<class ... T>
	struct BinaryDeserialise<std::tuple<T...>>
	{
		BinaryDeserialise(const std::vector<char>& someBinaryData, size_t& aDataIterator, std::tuple<T...>& someOutData)
		{
			TupleDeserialise(someBinaryData, aDataIterator, someOutData, std::make_index_sequence<sizeof...(T)>{});
		}
	private:

		template<size_t ... IndexSequence>
		inline void TupleDeserialise(const std::vector<char>& someBinaryData, size_t& aDataIterator, std::tuple<T...>& someOutData, const std::index_sequence<IndexSequence...>&)
		{
			(BinaryDeserialise<T>(someBinaryData, aDataIterator, std::get<IndexSequence>(someOutData)), ...);
		}
	};

	template<class FirstType, class ... TypeList>
	struct PackedSerialise
	{
		PackedSerialise(std::vector<char>& someBinaryData, size_t& aDataIterator, const FirstType& aFirstElement, const TypeList& ... aParamList)
		{
			BinarySerialise<FirstType>(someBinaryData, aDataIterator, aFirstElement);
			if constexpr ((sizeof...(TypeList)) > 0)
			{
				PackedSerialise<TypeList...>(someBinaryData, aDataIterator, aParamList ...);
			}
		}
	};

	template<class FirstType, class ... TypeList>
	struct PackedDeserialise
	{
		PackedDeserialise(std::vector<char>& someBinaryData, size_t& aDataIterator, FirstType& aFirstElementOut, TypeList& ... aParamListOut)
		{
			BinaryDeserialise<FirstType>(someBinaryData, aDataIterator, aFirstElementOut);
			if constexpr ((sizeof...(TypeList)) > 0)
			{
				PackedDeserialise<TypeList...>(someBinaryData, aDataIterator, aParamListOut ...);
			}
		}
	};
}
namespace CU = CommonUtility;
