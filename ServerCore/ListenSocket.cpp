#include "pch.h"
#include "ListenSocket.h"

ListenSocket::ListenSocket() : SocketBase()
{
	cout << "府郊家南 积己" << endl;
}

ListenSocket::~ListenSocket()
{
}

int ListenSocket::Listen(int backlog)
{
	if (listen(GetSocket(), backlog) == SOCKET_ERROR)
	{
		cout << "listen fail" << endl;
		return 1;
	}

	return 0;
}

SOCKET ListenSocket::Accept(NetAddress clientNetAddress)
{
	SOCKET socket = accept(GetSocket(), (struct sockaddr*)&clientNetAddress.GetSockAddr(), &clientNetAddress.GetLength());
	return socket;
}
