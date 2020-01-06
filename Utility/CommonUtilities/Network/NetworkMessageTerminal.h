#pragma once
#include "NetworkDefinitions.h"
#include "NetworkMessageBase.h"

class NetworkMessageTerminal
{
public:

	NetworkMessageTerminal() {}
	~NetworkMessageTerminal()
	{
		for (MessageContainer& m : myMessageContainers)
		{
			(this->*m.myDestructFunction)();
		}
	}

	template<class MessageType, class ... NetworkMessageTypes>
	void RegisterTypes();

	void StoreMessage(const std::vector<char>& aSerialisedMessage);

	template<class MessageType>
	void PackMessage(MessageType& aMessageToPack, const ClientID& aSender, const ClientID& aReceiver);

	template<class MessageType>
	const std::vector<MessageType>& GetMessages() const;

private:

	template<class MessageType>
	void RegisterType();

	template<class MessageType>
	void DecodeAndStore(const std::vector<char>& aSerialisedMessage);

	template<class MessageType>
	void OnDestruct();

	template<class MessageType>
	const bool ValidType() const;

	const bool ValidType(const MessageTypeIndex& aType) const;

	using NetworkMessageEnumerator = TemplateUtility::TypeFamily<NetworkMessageBase>;

	struct MessageContainer
	{
		void* myMessages;
		void(NetworkMessageTerminal::*myDecodeFunction)(const std::vector<char>&);
		void(NetworkMessageTerminal::*myDestructFunction)();
	};

	std::vector<MessageContainer> myMessageContainers;
};

template<class MessageType>
inline void NetworkMessageTerminal::RegisterType()
{
	static_assert(TU::Inherits<NetworkMessageBase, MessageType>(), "NetworkMessageTerminal::RegisterType is only legal for types that inherits from class NetworkMessageBase.");
	const MessageTypeIndex typeIndex = NetworkMessageEnumerator::type<MessageType>;

	assert(!ValidType<MessageType>() && "Tried to register network message type more than once to the terminal, register types only once!");

	myMessageContainers.resize(typeIndex + 1);
	MessageContainer& container = myMessageContainers[typeIndex];

	container.myMessages = new std::vector<MessageType>();
	container.myDecodeFunction = &NetworkMessageTerminal::DecodeAndStore<MessageType>;
	container.myDestructFunction = &NetworkMessageTerminal::OnDestruct<MessageType>;
}

template<class MessageType>
inline void NetworkMessageTerminal::DecodeAndStore(const std::vector<char>& aSerialisedMessage)
{
	assert(ValidType<MessageType>() && "NetworkMessageTerminal tried to decode and store a message of unknown type. Make sure to register the type on startup!");

	const MessageTypeIndex typeIndex = NetworkMessageEnumerator::type<MessageType>;
	MessageContainer& container = myMessageContainers[typeIndex];
	std::vector<MessageType>& messages = *static_cast<std::vector<MessageType>*>(container.myMessages);
	
	messages.push_back(MessageType());
	MessageType& messageToStore = messages[messages.size() - 1];
	messageToStore.DeserialiseMessage(aSerialisedMessage);

}

template<class MessageType>
inline void NetworkMessageTerminal::OnDestruct()
{
	const MessageTypeIndex typeIndex = NetworkMessageEnumerator::type<MessageType>;
	delete static_cast<std::vector<MessageType>*>(myMessageContainers[typeIndex].myMessages);
}

template<class MessageType, class ...NetworkMessageTypes>
inline void NetworkMessageTerminal::RegisterTypes()
{
	RegisterType<MessageType>();
	if constexpr ((sizeof...(NetworkMessageTypes)) > 0)
	{
		RegisterTypes<NetworkMessageTypes...>();
	}
}

template<class MessageType>
inline void NetworkMessageTerminal::PackMessage(MessageType & aMessageToPack, const ClientID & aSender, const ClientID & aReceiver)
{
	static_assert(TU::Inherits<NetworkMessageBase, MessageType>(), "NetworkMessageTerminal::PackMessage is only legal for types that inherits from class NetworkMessageBase.");
	assert(ValidType<MessageType>() && "Tried to pack a network message of invalid type, make sure it is registered with the NetworkMessageTerminal::RegisterTypes function.");

	const MessageTypeIndex typeIndex = NetworkMessageEnumerator::type<MessageType>;
	aMessageToPack.BuildMessage(aSender, aReceiver, typeIndex);
}

template<class MessageType>
inline const std::vector<MessageType>& NetworkMessageTerminal::GetMessages() const
{
	static_assert(TU::Inherits<NetworkMessageBase, MessageType>(), "NetworkMessageTerminal::GetMessages is only legal for types that inherits from class NetworkMessageBase.");
	assert(ValidType<MessageType>() && "Tried to pack a network message of invalid type, make sure it is registered with the NetworkMessageTerminal::RegisterTypes function.");
	const MessageTypeIndex typeIndex = NetworkMessageEnumerator::type<MessageType>;

	return *static_cast<std::vector<MessageType>*>(myMessageContainers[typeIndex].myMessages);
}

inline void NetworkMessageTerminal::StoreMessage(const std::vector<char>& aSerialisedMessage)
{
	NetworkMessageHeader header;
	size_t headerIterator = 0;
	CU::BinaryDeserialise<NetworkMessageHeader>(aSerialisedMessage, headerIterator, header);
	assert(ValidType(header.myMessageType) && "NetworkMessageTerminal tried to decode a network message of unknown type.");

	MessageContainer& container = myMessageContainers[header.myMessageType];
	(this->*container.myDecodeFunction)(aSerialisedMessage);
	
}

inline const bool NetworkMessageTerminal::ValidType(const MessageTypeIndex & aType) const
{
	return (aType >= 0) && (aType < static_cast<MessageTypeIndex>(myMessageContainers.size()));
}

template<class MessageType>
inline const bool NetworkMessageTerminal::ValidType() const
{
	const MessageTypeIndex typeIndex = NetworkMessageEnumerator::type<MessageType>;
	return (typeIndex >= 0) && (typeIndex < static_cast<MessageTypeIndex>(myMessageContainers.size()));
}
