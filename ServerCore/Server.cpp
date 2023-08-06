#include "pch.h"
#include "Server.h"

Server::Server()
{
	WSADATA WSAData;
	if (::WSAStartup(MAKEWORD(2, 2), &WSAData) == SOCKET_ERROR)
	{
		printf("Server - WSAStartup fail");
	}
}

Server::~Server()
{
	::WSACleanup();
}

bool Server::Run(int port, int backlog)
{
	if (mIocp.InitSocket() == false)
	{
		printf("Server Run - InitSocket Fail\n");
	}
	
	if (mIocp.BindAndListen(port, backlog) == false)
	{
		printf("Server Run - BindAndListen(%d, %d) Fail\n", port, backlog);
	}
	
	if (mIocp.InitIocp() == false) {
		printf("Server Run -InitIOCP Fail\n");
	}

	mIocp.Run();
	
	return true;
}
