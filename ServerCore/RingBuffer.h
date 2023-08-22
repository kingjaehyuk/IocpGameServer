#pragma once

class RingBuffer
{
public:
	RingBuffer(int capacity);
	virtual ~RingBuffer();

	int Write(const char* data, int size);
	int Read(char* dest, int size);
	int Peek(char* dest, int size);
	void Clear();
	void MoveWriteCursor(int size);
	void MoveReadCursor(int size);

	char* GetBuffer() const { return mBuffer; }
	char* GetWriteCursor() const { return mWriteCursor; }
	char* GetReadCursor() const { return mReadCursor; }
	char* GetBegin() const { return mBegin; }
	char* GetEnd() const { return mEnd; }

	int GetCapacity() const;
	int GetUsedSize() const;
	int GetFreeSize() const;
	int GetUnbrokenWriteSize() const;
	int GetUnbrokenReadSize() const;

private:
	char* mBuffer;
	char* mWriteCursor;
	char* mReadCursor;
	char* mBegin;
	char* mEnd;
};

