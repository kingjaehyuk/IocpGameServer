#include "pch.h"
#include "RingBuffer.h"

RingBuffer::RingBuffer(int size)
{
	mBuffer = new char[size];
	mReadCursor = mBuffer;
	mWriteCursor = mBuffer;

	mBegin = mBuffer;
	mEnd = mBuffer + size;
}

RingBuffer::~RingBuffer()
{
	delete[] mBuffer;
}

int RingBuffer::Push(char* data, int size)
{
	if (size > GetRemainingSize())
	{
		return 1;
	}

	if (mWriteCursor > mReadCursor)
	{
		if (mEnd - mWriteCursor >= size)
		{
			memcpy_s(mWriteCursor, size, data, size);
			MoveWriteCursor(size);
			return 0;
		}
	}
	else
	{

	}
	

	return 0;
}

int RingBuffer::Pop(char* data, int size)
{
	return 0;
}

int RingBuffer::GetCapacity()
{
	return static_cast<int>(mEnd - mBegin);
}

int RingBuffer::GetUsedSize()
{
	if (mReadCursor > mWriteCursor)
	{
		return static_cast<int>((mEnd - mReadCursor) + (mWriteCursor - mBegin));
	}
	return static_cast<int>(mWriteCursor - mReadCursor);
}

int RingBuffer::GetRemainingSize()
{
	if (mWriteCursor > mReadCursor)
	{
		return static_cast<int>((mEnd - mWriteCursor) + (mReadCursor - mBegin));
	}
	return static_cast<int>(mReadCursor - mWriteCursor);
}

void RingBuffer::MoveWriteCursor(int size)
{
	if (mWriteCursor + size > mEnd)
	{
		mWriteCursor = mBegin + (mWriteCursor + size - mEnd);
	}
	else
	{
		mWriteCursor += size;
	}
}
