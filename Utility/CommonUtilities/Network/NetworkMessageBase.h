#pragma once
#include <vector>
#include "NetworkDefinitions.h"
#include "BinarySerialiser.h"

class NetworkMessageBase
{
public:

	NetworkMessageBase() {}
	virtual ~NetworkMessageBase() {}

	const std::vector<char>& GetBinaryData() const;

	void BuildMessage(const ClientID& aSender, const ClientID& aReceiver, const MessageTypeIndex& aTypeID);
	void DeserialiseMessage(const std::vector<char>& someBinaryData);

private:
	friend class NetworkMessageTerminal;

	
	

	enum class MessageFlagInternal
	{
		Write,
		Read,
		Count
	};

	virtual void SerialiseInternal(std::vector<char>& someBinaryData, size_t& aDataIterator) = 0;
	virtual void DeserialiseInternal(const std::vector<char>& someBinaryData, size_t& aDataIterator) = 0;

	NetworkMessageHeader myMessageHeader;

	std::vector<char> myBinaryData;
	size_t myDataIterator;
	MessageFlagInternal myFlag = MessageFlagInternal::Count;

	inline static MessageID ourMessageIterator = 0;

};

inline const std::vector<char>& NetworkMessageBase::GetBinaryData() const
{
	return myBinaryData;
}

inline void NetworkMessageBase::BuildMessage(const ClientID & aSender, const ClientID & aReceiver, const MessageTypeIndex& aTypeID)
{
	assert(myFlag != MessageFlagInternal::Write && "Tried to build a network message twice.");
	assert(myFlag != MessageFlagInternal::Read && "Tried to build a network message received from another network client.");
	myFlag = MessageFlagInternal::Write;

	myMessageHeader.mySenderID = aSender;
	myMessageHeader.myReceiverID = aReceiver;
	myMessageHeader.myMessageID = ourMessageIterator++;
	myMessageHeader.myMessageType = aTypeID;
	myMessageHeader.myTimestamp = 0.0;

	myDataIterator = 0;

	CU::BinarySerialise<NetworkMessageHeader>(myBinaryData, myDataIterator, myMessageHeader);
	SerialiseInternal(myBinaryData, myDataIterator);
}

inline void NetworkMessageBase::DeserialiseMessage(const std::vector<char>& someBinaryData)
{
	assert(myFlag != MessageFlagInternal::Write && "Tried to deserialise a network message received from another network client.");
	assert(myFlag != MessageFlagInternal::Read && "Tried to deserialise a network message twice.");
	myFlag = MessageFlagInternal::Read;

	myDataIterator = 0;

	CU::BinaryDeserialise<NetworkMessageHeader>(someBinaryData, myDataIterator, myMessageHeader);
	DeserialiseInternal(someBinaryData, myDataIterator);
}