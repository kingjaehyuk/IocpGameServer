#pragma once
#include "SocketBase.h"

constexpr int MAX_BUFFER = 1024;

class Buffer : public OVERLAPPED
{
public:
	//~Buffer()
	//{
	//	//delete Pointer;
	//	//delete hEvent;
	//	delete[] messageBuffer;
	//};

public:
	WSABUF wsaBuffer;
	char messageBuffer[MAX_BUFFER];
	eOperationType GetOperationType() { return mOperationType; }
	void SetOperationType(eOperationType operationType) { mOperationType = operationType; }
private:
	eOperationType mOperationType;

};

class Session : public SocketBase
{
public:
	Session(int id);
	Session(int id, SOCKET socket);
	void Init(eOperationType operationType = eOperationType::None);

public:
	int GetId() { return mId; }
	//
	bool Recv();
	int Send(const char* data, int size);

public:
	Buffer recvBuffer;
	DWORD receiveBytes = 0;

private:
	int mId;
};

