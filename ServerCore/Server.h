#pragma once
#include "IOCP.h"

constexpr int MAX_RESERVE_SESSION_COUNT = 1;

class ListenScoket;
class Session;

class Server
{
public:
	Server(int port, int backlog);
	virtual ~Server();

	int Init();

	bool Run();

	Session* GetReserveSession();
	void PopReserveSession();
	int AddSession();

private:
	int InitSocket();
	int BindAndListen();
private:
	int mPort;
	int mBacklog;

	IOCP mIocp;
	ListenSocket* mListenSocket;
	set<Session*> mSessionList;
	queue<Session*> mReserveSessionList;

	int mSessionId = 0;
};

