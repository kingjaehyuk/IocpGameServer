#include "pch.h"
#include "AcceptEvent.h"

AcceptEvent::AcceptEvent() : IocpEvent(eIocpEventType::Accept)
{
}

AcceptEvent::~AcceptEvent()
{
}
