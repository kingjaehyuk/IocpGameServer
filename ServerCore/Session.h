#pragma once
#include "SocketBase.h"
#include "RecvBuffer.h"
#include "RecvEvent.h"
#include "SendEvent.h"

//constexpr int MAX_BUFFER = 1024;
constexpr int MAX_BUFFER_CAPACITY = 1024;

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
	RecvBuffer GetRecvBuffer() { return mRecvBuffer; }

	int RegisterRecv();
	int HandleRecv(int bytesTransferred);
	//
	//int RegisterSend();
	//int HandleSend();

	//int Recv();
	int Send(const char* data, int size);
//public:
//	virtual int OnRecv(BYTE* buffer, int size);

public:
	//Buffer* recvBuffer;
	DWORD receiveBytes = 0;

private:
	int mId;
	Server& mServer;
	
	RecvBuffer mRecvBuffer;
	RecvEvent mRecvEvent;
	SendEvent mSendEvent;
};

