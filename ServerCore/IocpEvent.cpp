#include "pch.h"
#include "IocpEvent.h"

IocpEvent::IocpEvent(eIocpEventType eventType) : mEventType(eventType)
{
	Init();
}

void IocpEvent::Init()
{
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
	hEvent = nullptr;
}
