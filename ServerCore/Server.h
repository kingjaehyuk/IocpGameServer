#pragma once
#include "IOCP.h"
class Server
{
public:
	Server();
	virtual ~Server();
	bool Run(int port, int backlog);
private:
	IOCP mIocp;
};

