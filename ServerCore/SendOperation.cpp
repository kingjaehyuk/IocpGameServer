#include "pch.h"
#include "SendOperation.h"

SendOperation::SendOperation() : IocpOperation(eOperationType::Send)
{
}

SendOperation::~SendOperation()
{
}
