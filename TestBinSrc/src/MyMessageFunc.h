#pragma once
#include "MessageFunc.h"
class MyMessageFunc : public MessageFunc
{
	public:
		void AcceptMessage(const Message& message) override;
};