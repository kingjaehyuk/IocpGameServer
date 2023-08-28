#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int capacity) : mReadPos(0), mWritePos(0)
{
	mBuffer.resize(capacity);
}

RecvBuffer::~RecvBuffer()
{
}

BYTE* RecvBuffer::GetReadCursor()
{
	return &mBuffer[mReadPos];
}

BYTE* RecvBuffer::GetWriteCursor()
{
	return &mBuffer[mWritePos];
}

void RecvBuffer::Clean()
{
	int usedSize = GetUsedSize();
	if (usedSize == 0)
	{
		mReadPos = 0;
		mWritePos = 0;
	}
	else
	{
		::memcpy(&mBuffer[0], &mBuffer[mReadPos], usedSize);
		mReadPos = 0;
		mWritePos = usedSize;
	}
}

int RecvBuffer::GetUsedSize() const
{
	return mWritePos - mReadPos;
}

int RecvBuffer::GetFreeSize() const
{
	return mBuffer.capacity() - mWritePos;
}

int RecvBuffer::OnRead(int size)
{
	if (size > GetUsedSize())
	{
		return 0;
	}

	mReadPos += size;

	return size;
}

int RecvBuffer::OnWrite(int size)
{
	if (size > GetFreeSize())
	{
		return 0;
	}

	mWritePos += size;

	return size;
}
