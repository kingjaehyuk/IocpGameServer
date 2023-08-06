#pragma once
#include "SocketBase.h"
class ListenSocket : public SocketBase
{
public:
	ListenSocket();
	virtual ~ListenSocket();

	int Listen(int backlog);
	SOCKET Accept(NetAddress clientNetAddress);
private:
};

