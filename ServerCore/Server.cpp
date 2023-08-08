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

	return 0;
}

int Server::InitSocket()
{
	mListenSocket = new ListenSocket;

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


bool Server::Run()
{
	// Init �Ⱥθ���
	assert(mListenSocket != nullptr);

	if (mIocp.Init())
	{
		printf("IOCP Ŭ���� ���� - IOCP �ʱ�ȭ ����");
		return false;
	}

	mIocp.Run(mListenSocket);
	
	return true;
}
