#pragma once
constexpr int MAX_RING_BUFFER_SIZE = 1024;
class RingBuffer
{
public:
	RingBuffer(int size = MAX_RING_BUFFER_SIZE);
	virtual ~RingBuffer();

	int Push(char* data, int size);
	int Pop(char* data, int size);

	int GetCapacity();
	int GetUsedSize();
	int GetRemainingSize();

private:

	void MoveWriteCursor(int size);

	char* mBuffer;
	char* mReadCursor;
	char* mWriteCursor;
	char* mBegin;
	char* mEnd;
};

