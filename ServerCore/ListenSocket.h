#pragma once
#include "SocketBase.h"
#include "Buffer.h"
#include "AcceptEvent.h"

class Server;

class ListenSocket : public SocketBase
{
public:
	ListenSocket(Server& server);
	virtual ~ListenSocket();

	int Listen(int backlog);
	Server& GetServer() { return mServer; };

	int RegisterAccept();
	int HandleAccept();
public:
	//Buffer* acceptBuffer;
	//DWORD acceptBytes = 0;

private:
	AcceptEvent* mAcceptEvent;
	Server& mServer;
};

