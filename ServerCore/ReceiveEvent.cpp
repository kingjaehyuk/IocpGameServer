#include "pch.h"
#include "ReceiveEvent.h"

ReceiveEvent::ReceiveEvent() : IocpEvent(eIocpEventType::Recv)
{
}

ReceiveEvent::~ReceiveEvent()
{
}
