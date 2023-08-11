#include "pch.h"
#include "Server.h"

Server::Server(int port, int backlog) : mPort(port), mBacklog(backlog), mListenSocket(nullptr)
{
	WSADATA WSAData;
	if (::WSAStartup(MAKEWORD(2, 2), &WSAData) == SOCKET_ERROR)
	{
		printf("Server - WSAStartup fail");
	}
}

Server::~Server()
{
	delete mListenSocket;

	::WSACleanup();
}

int Server::Init()
{
	if (InitSocket())
	{
		return 1;
	}

	if (BindAndListen())
	{
		return 1;
	}

	if (mIocp.Init())
	{
		printf("IOCP Ŭ���� ���� - IOCP �ʱ�ȭ ����");
		return 1;
	}

	mIocp.Add(*mListenSocket);

	for (int i = 0; i < MAX_RESERVE_SESSION_COUNT; i++)
	{
		mReserveSessionList.push(new Session(*this, ++mSessionId));
	}

	return 0;
}

int Server::InitSocket()
{
	mListenSocket = new ListenSocket(*this);

	if (mListenSocket->GetSocket() == INVALID_SOCKET)
	{
		printf("Server Ŭ���� ���� - ���� ���� ���� ����");
		return 1;
	}

	return 0;
}

int Server::BindAndListen()
{
	NetAddress serverAddr(mPort);

	if (mListenSocket->Bind(serverAddr) == SOCKET_ERROR)
	{
		printf("Server Ŭ���� ���� - ���� ���� ���ε� ����");
		return 1;
	};

	if (mListenSocket->Listen(mBacklog) == SOCKET_ERROR)
	{
		printf("Server Ŭ���� ���� - ���� ���� ���� ����");
		return 1;
	};

	return 0;
}

Session* Server::GetReserveSession()
{
	if (mReserveSessionList.size() < MAX_RESERVE_SESSION_COUNT) {
		mReserveSessionList.push(new Session(*this, ++mSessionId));
	}

	return mReserveSessionList.front();
}

void Server::PopReserveSession()
{
	mReserveSessionList.pop();
}

int Server::AddSession()
{
	Session* session = GetReserveSession();
	mIocp.Add(*session);

	mSessionList.insert(session);

	session->Recv();

	PopReserveSession();
	return 0;
}

bool Server::Run()
{
	// Init �Ⱥθ���
	assert(mListenSocket != nullptr);

	mListenSocket->RegisterAccept();
	mIocp.Run();
	
	return true;
}
