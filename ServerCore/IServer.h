#pragma once
class IServer
{
public:
	IServer() {}
	virtual ~IServer() {};

	virtual IServer& GetServer() = 0;
};

