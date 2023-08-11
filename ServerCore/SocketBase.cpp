#include "pch.h"
#include "SocketBase.h"

SocketBase::SocketBase() : mSocket(WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED))
{
	cout << "家南海捞胶 积己" << endl;
}

SocketBase::SocketBase(SOCKET socket) : mSocket(socket)
{
	cout << "家南海捞胶 积己" << endl;
}

SocketBase::~SocketBase()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
	}
}

int SocketBase::Bind(NetAddress netAddress)
{
	return bind(mSocket, reinterpret_cast<const sockaddr*>(&netAddress.GetSockAddr()), sizeof(SOCKADDR_IN));
}

int SocketBase::SetSockOpt(SOCKET listenSocket)
{
	if (::setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&listenSocket), sizeof(listenSocket)))
	{
		return 1;
	}

	return 0;
}

void SocketBase::SetSocket(SOCKET socket)
{
	mSocket = socket;
}
