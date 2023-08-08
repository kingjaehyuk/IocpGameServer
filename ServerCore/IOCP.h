#pragma once
#include "ListenSocket.h"
#include "Session.h"

class IOCP
{
public:
	IOCP();
	virtual ~IOCP();

	//bool InitSocket();
	//bool BindAndListen(int port, int backlog);
	int Init();
	void Run(ListenSocket*& listenSocket);
	void WorkerThread();
	void AcceptThread();
	HANDLE getIocpHandle() const { return mIOCP; }

private:
	//void shutdown(const char* message);

private:
	//ListenSocket* mListenSocket;
	vector<HANDLE> hThread;
	vector<thread*> mThreadPool;
	map<int, Session*> mSessionList;
	HANDLE mIOCP;
};

/*
#pragma pack(push, 1)
��ŶŬ����
��������: ����Ʈ������ ��������
#pragma pack(pop)
*/