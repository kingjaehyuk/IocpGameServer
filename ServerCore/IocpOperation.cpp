#include "pch.h"
#include "IocpOperation.h"

IocpOperation::IocpOperation(eOperationType operationType) : mOperationType(operationType)
{
	Init();
}

IocpOperation::~IocpOperation()
{
}

void IocpOperation::Init()
{
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
	hEvent = nullptr;
}
