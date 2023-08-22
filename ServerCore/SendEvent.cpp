#include "pch.h"
#include "SendEvent.h"

SendEvent::SendEvent() : IocpEvent(eIocpEventType::Send)
{
}

SendEvent::~SendEvent()
{
}
