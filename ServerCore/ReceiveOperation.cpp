#include "pch.h"
#include "ReceiveOperation.h"

ReceiveOperation::ReceiveOperation() : IocpOperation(eOperationType::Recv)
{
}

ReceiveOperation::~ReceiveOperation()
{
}
