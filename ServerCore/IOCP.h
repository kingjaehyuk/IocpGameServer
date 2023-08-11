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
��ŶŬ����
��������: ����Ʈ������ ��������
#pragma pack(pop)
*/