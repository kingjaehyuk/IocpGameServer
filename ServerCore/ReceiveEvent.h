#pragma once
#include "IocpEvent.h"

class ReceiveEvent : public IocpEvent
{
public:
	ReceiveEvent();
	virtual ~ReceiveEvent();
};

