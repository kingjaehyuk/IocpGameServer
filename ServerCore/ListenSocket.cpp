#include "pch.h"
#include "ListenSocket.h"
#include "Server.h"

ListenSocket::ListenSocket(Server& server) : SocketBase(), mServer(server)
{
	acceptBuffer = new Buffer(eOperationType::Accept);
	cout << "리슨소켓 생성" << endl;
}

ListenSocket::~ListenSocket()
{
	delete acceptBuffer;
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

int ListenSocket::RegisterAccept()
{
	Session* newSession = mServer.GetReserveSession();
	if (::AcceptEx(GetSocket(), newSession->GetSocket(), newSession->recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &acceptBytes, acceptBuffer))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING) {
			printf("세션 클래스 에러 - AcceptEx 실패 (%d)\n", WSAGetLastError());
			RegisterAccept();
			return 1;
		}
	}
	return 0;
}

int ListenSocket::Accept()
{
	if (mServer.GetReserveSession()->SetSockOpt(GetSocket()))
	{
		RegisterAccept();
		return 1;
	};

	if (mServer.AddSession())
	{
		RegisterAccept();
		return 1;
	}

	RegisterAccept();

	return 0;
}
