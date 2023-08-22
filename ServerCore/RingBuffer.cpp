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
	mWriteCursor = nullptr;
	mReadCursor = nullptr;
	mBegin = nullptr;
	mEnd = nullptr;

	if (mBuffer != nullptr)
		delete[] mBuffer;
	mBuffer = nullptr;
}

int RingBuffer::Write(const char* data, int size)
{
	int freeSize = GetFreeSize();
	if (size > freeSize)
	{
		return 0;
	}

	int unbrokenWriteSize = GetUnbrokenWriteSize();

	if (unbrokenWriteSize >= size)
	{
		memcpy_s(mWriteCursor, size, data, size);
		MoveWriteCursor(size);
		return size;
	}

	memcpy_s(mWriteCursor, unbrokenWriteSize, data, unbrokenWriteSize);
	MoveWriteCursor(unbrokenWriteSize);

	int remainingSize = size - unbrokenWriteSize;
	memcpy_s(mWriteCursor, remainingSize, data + unbrokenWriteSize, remainingSize);
	MoveWriteCursor(remainingSize);

	return size;
}

int RingBuffer::Read(char* dest, int size)
{
	int usedSize = GetUsedSize();
	if (size > usedSize)
	{
		return 0;
	}

	int unbrokenReadSize = GetUnbrokenReadSize();

	if (unbrokenReadSize <= size)
	{
		memcpy_s(dest, size, mReadCursor, size);
		MoveReadCursor(size);
		return size;
	}

	memcpy_s(dest, unbrokenReadSize, mReadCursor, unbrokenReadSize);
	MoveReadCursor(unbrokenReadSize);

	int remainingSize = size - unbrokenReadSize;
	memcpy_s(dest + unbrokenReadSize, remainingSize, mReadCursor, remainingSize);
	MoveReadCursor(remainingSize);

	return size;
}

int RingBuffer::Peek(char* dest, int size)
{
	int usedSize = GetUsedSize();
	if (size > usedSize)
	{
		return 0;
	}

	int unbrokenReadSize = GetUnbrokenReadSize();

	if (unbrokenReadSize <= size)
	{
		memcpy_s(dest, size, mReadCursor, size);
		return size;
	}

	memcpy_s(dest, unbrokenReadSize, mReadCursor, unbrokenReadSize);

	int remainingSize = size - unbrokenReadSize;
	memcpy_s(dest + unbrokenReadSize, remainingSize, mReadCursor, remainingSize);

	return size;
}

void RingBuffer::Clear()
{
	mWriteCursor = mBegin;
	mReadCursor = mBegin;
}

int RingBuffer::GetCapacity() const
{
	return static_cast<int>(mEnd - mBegin);
}

int RingBuffer::GetUsedSize() const
{
	if (mReadCursor > mWriteCursor)
	{
		return static_cast<int>((mEnd - mReadCursor) + (mWriteCursor - mBegin));
	}
	return static_cast<int>(mWriteCursor - mReadCursor);
}

int RingBuffer::GetFreeSize() const
{
	if (mWriteCursor >= mReadCursor)
	{
		return static_cast<int>((mEnd - mWriteCursor) + (mReadCursor - mBegin));
	}
	return static_cast<int>(mReadCursor - mWriteCursor);
}

int RingBuffer::GetUnbrokenWriteSize() const
{
	if (mWriteCursor > mReadCursor)
	{
		return static_cast<int>(mEnd - mWriteCursor);
	}

	return static_cast<int>(mReadCursor - mWriteCursor);
}

int RingBuffer::GetUnbrokenReadSize() const
{
	if (mWriteCursor > mReadCursor)
	{
		return static_cast<int>(mWriteCursor - mReadCursor);
	}

	return static_cast<int>(mEnd - mReadCursor);
}

void RingBuffer::MoveWriteCursor(int size)
{
	mWriteCursor += size;

	if (mWriteCursor > mEnd)
	{
		mWriteCursor = mBegin + (mWriteCursor - mEnd);
	}
}

void RingBuffer::MoveReadCursor(int size)
{
	mReadCursor += size;

	if (mReadCursor > mEnd)
	{
		mReadCursor = mBegin + (mReadCursor - mEnd);
	}
}
