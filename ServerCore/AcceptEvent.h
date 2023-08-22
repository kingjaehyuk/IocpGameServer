#pragma once
#include "IocpEvent.h"

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent();
	virtual ~AcceptEvent();
};

