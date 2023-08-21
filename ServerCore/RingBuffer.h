#pragma once
constexpr int MAX_RING_BUFFER_SIZE = 1024;
class RingBuffer
{
public:
	RingBuffer(int size = MAX_RING_BUFFER_SIZE);
	virtual ~RingBuffer();

	int Write(const char* data, int size);
	int Read(char* dest, int size);
	int Peek(char* dest, int size);

	char* GetWriteCursor() const;
	char* GetReadCursor() const;
	int GetCapacity() const;
	int GetUsedSize() const;
	int GetFreeSize() const;
	int GetUnbrokenWriteSize() const;
	int GetUnbrokenReadSize() const;

private:
	void MoveWriteCursor(int size);
	void MoveReadCursor(int size);

	char* mBuffer;
	char* mWriteCursor;
	char* mReadCursor;
	char* mBegin;
	char* mEnd;
};

