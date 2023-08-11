#pragma once
#include "NetAddress.h"
class SocketBase
{
public:
	SocketBase();
	SocketBase(SOCKET socket);
	virtual ~SocketBase();

public:
	int Bind(NetAddress netAddress);
	SOCKET& GetSocket() { return mSocket; }
	int SetSockOpt(SOCKET listenSocket);

protected:
	void SetSocket(SOCKET socket);

private:
	SOCKET mSocket;
};

