#pragma once
#include "TemplateUtility/TypeFamily.h"
#include <assert.h>

struct NetworkMessageHeader;
using ClientID = unsigned int;
using MessageID = unsigned int;
using MessageTypeIndex = TemplateUtility::TypeFamily<NetworkMessageHeader>::family_type;
using NetworkTimestamp = double;

struct NetworkMessageHeader
{
	ClientID mySenderID;
	ClientID myReceiverID;
	MessageID myMessageID;
	MessageTypeIndex myMessageType;
	NetworkTimestamp myTimestamp;
};