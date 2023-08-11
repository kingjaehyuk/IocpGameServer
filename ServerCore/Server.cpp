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
		printf("IOCP 클래스 에러 - IOCP 초기화 실패");
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
		printf("Server 클래스 에러 - 리슨 소켓 생성 실패");
		return 1;
	}

	return 0;
}

int Server::BindAndListen()
{
	NetAddress serverAddr(mPort);

	if (mListenSocket->Bind(serverAddr) == SOCKET_ERROR)
	{
		printf("Server 클래스 에러 - 리슨 소켓 바인드 실패");
		return 1;
	};

	if (mListenSocket->Listen(mBacklog) == SOCKET_ERROR)
	{
		printf("Server 클래스 에러 - 리슨 소켓 리슨 실패");
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
	// Init 안부른듯
	assert(mListenSocket != nullptr);

	mListenSocket->RegisterAccept();
	mIocp.Run();
	
	return true;
}
