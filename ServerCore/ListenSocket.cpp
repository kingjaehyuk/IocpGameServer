#include "pch.h"
#include "ListenSocket.h"
#include "Server.h"

ListenSocket::ListenSocket(Server& server) : SocketBase(), mServer(server)
{
	//acceptBuffer = new Buffer(eOperationType::Accept);
	mAcceptEvent = new AcceptEvent();
	cout << "府郊家南 积己" << endl;
}

ListenSocket::~ListenSocket()
{
	delete mAcceptEvent;
	printf("府郊家南 昏力\n");
	//delete acceptBuffer;
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
	DWORD bytesReceived = 0;

	mAcceptEvent->Init();

	if (::AcceptEx(GetSocket(), newSession->GetSocket(), newSession->GetRecvBuffer().GetWriteCursor(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytesReceived, mAcceptEvent))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING) {
			printf("技记 努贰胶 俊矾 - AcceptEx 角菩 (%d)\n", WSAGetLastError());
			RegisterAccept();
			return 1;
		}
	}
	return 0;
}

int ListenSocket::HandleAccept()
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
