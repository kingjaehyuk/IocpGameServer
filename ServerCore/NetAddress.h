#pragma once
class NetAddress
{
public:
	NetAddress();
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(int port);
	NetAddress(const char* ip, int port);

	SOCKADDR_IN& GetSockAddr() { return mSockAddr; }
	int& GetLength() { return mLength; }
private:
	SOCKADDR_IN mSockAddr = { 0 };
	int mLength = sizeof(SOCKADDR_IN);;
};

