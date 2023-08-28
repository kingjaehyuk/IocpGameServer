#pragma once
#include "IocpEvent.h"

class SendEvent :public IocpEvent
{
public:
	SendEvent();
public:
	char* Buffer;
};

