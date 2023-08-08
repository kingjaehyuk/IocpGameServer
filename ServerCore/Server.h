#pragma once
#include "IOCP.h"
class Server
{
public:
	Server(int port, int backlog);
	virtual ~Server();

	int Init();

	bool Run();

private:
	int InitSocket();
	int BindAndListen();

private:
	int mPort;
	int mBacklog;

	IOCP mIocp;
	ListenSocket* mListenSocket;
};

