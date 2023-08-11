#pragma once
#include "Session.h"
#include "ListenSocket.h"

class IOCP
{
public:
	IOCP();
	virtual ~IOCP();

	int Init();
	void Run();
	void WorkerThread();
	HANDLE GetIocpHandle() const { return mIocpHandle; }
	void Add(ListenSocket& listenSocket);
	void Add(Session& session);


private:
	vector<HANDLE> hThread;
	vector<thread*> mThreadPool;
	HANDLE mIocpHandle;
};

/*
#pragma pack(push, 1)
패킷클래스
쓰는이유: 바이트정렬을 막기위함
#pragma pack(pop)
*/