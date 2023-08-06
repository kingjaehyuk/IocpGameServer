#include "pch.h"
#include "SocketBase.h"

SocketBase::SocketBase() : mSocket(WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED))
{
	cout << "���Ϻ��̽� ����" << endl;
}

SocketBase::SocketBase(SOCKET socket) : mSocket(socket)
{
	cout << "���Ϻ��̽� ����" << endl;
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

int SocketBase::Close()
{
	if (mSocket != INVALID_SOCKET)
	{
		return closesocket(mSocket);
	}
	
	return SOCKET_ERROR;
}

void SocketBase::SetSocket(SOCKET socket)
{
	mSocket = socket;
}
