#pragma once
#include "SocketBase.h"
#include "Buffer.h"

//constexpr int MAX_BUFFER = 1024;

class Server;

//class Buffer : public OVERLAPPED
//{
//public:
//	//~Buffer()
//	//{
//	//	//delete Pointer;
//	//	//delete hEvent;
//	//	delete[] messageBuffer;
//	//};
//
//public:
//	WSABUF wsaBuffer;
//	char messageBuffer[MAX_BUFFER];
//	eOperationType GetOperationType() { return mOperationType; }
//	void SetOperationType(eOperationType operationType) { mOperationType = operationType; }
//private:
//	eOperationType mOperationType;
//
//};

class Session : public SocketBase
{
public:
	Session(Server& server);
	Session(Server& server, int id);
	Session(Server& server, int id, SOCKET socket);
	//void Init(eOperationType operationType = eOperationType::None);
	virtual ~Session();
public:
	int GetId() { return mId; }

	int Recv();
	int Send(const char* data, int size);
public:
	Buffer* recvBuffer;
	DWORD receiveBytes = 0;

private:
	int mId;
	Server& mServer;
};

