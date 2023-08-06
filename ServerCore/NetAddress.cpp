#include "pch.h"
#include "NetAddress.h"

NetAddress::NetAddress()
{
	ZeroMemory(&mSockAddr, sizeof(SOCKADDR_IN));
}

NetAddress::NetAddress(SOCKADDR_IN sockAddr) : mSockAddr(sockAddr)
{
}

NetAddress::NetAddress(int port)
{
	ZeroMemory(&mSockAddr, sizeof(SOCKADDR_IN));
	mSockAddr.sin_family = PF_INET;
	mSockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	mSockAddr.sin_port = htons(port);

}

NetAddress::NetAddress(const char* ip, int port)
{
	ZeroMemory(&mSockAddr, sizeof(SOCKADDR_IN));
	mSockAddr.sin_family = PF_INET;
	mSockAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	mSockAddr.sin_port = htons(port);
}
