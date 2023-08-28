#pragma once
class RecvBuffer
{
public:
	RecvBuffer(int capacity);
	virtual ~RecvBuffer();

	BYTE* GetReadCursor();
	BYTE* GetWriteCursor();

	void Clean();

	int GetUsedSize() const;
	int GetFreeSize() const;

	int OnRead(int size);
	int OnWrite(int size);
private:
	int mReadPos;
	int mWritePos;
	vector<BYTE> mBuffer;
};

