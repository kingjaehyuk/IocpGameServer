#include "pch.h"
#include "Session.h"
#include "Server.h"

Session::Session(Server& server) : SocketBase(), mId(0), mServer(server)
{
	recvBuffer = new Buffer(eOperationType::Recv);
}

Session::Session(Server& server, int id) : SocketBase(), mId(id), mServer(server)
{
	recvBuffer = new Buffer(eOperationType::Recv);
	printf("���� ���� ID: %d\n", mId);
}

Session::Session(Server& server, int id, SOCKET clientSocket) : SocketBase(clientSocket), mId(id), mServer(server)
{
	recvBuffer = new Buffer(eOperationType::Recv);
	printf("���� ���� ID: %d\n", mId);
}

Session::~Session()
{
	delete recvBuffer;
}

//void Session::Init(eOperationType operationType)
//{
//	if (operationType == eOperationType::None || operationType == eOperationType::Recv || operationType == eOperationType::Accept)
//	{
//		ZeroMemory(&recvBuffer, sizeof(Buffer));
//		ZeroMemory(&recvBuffer.messageBuffer, MAX_BUFFER);
//		recvBuffer.wsaBuffer.len = MAX_BUFFER;
//		recvBuffer.wsaBuffer.buf = recvBuffer.messageBuffer;
//		recvBuffer.SetOperationType(operationType);
//	}
//	//else if (operationType == eOperationType::Send)
//	//{
//	//	ZeroMemory(&sendBuffer, sizeof(Buffer));
//	//	ZeroMemory(&sendBuffer.messageBuffer, MAX_BUFFER);
//	//	sendBuffer.wsaBuffer.len = MAX_BUFFER;
//	//	sendBuffer.wsaBuffer.buf = sendBuffer.messageBuffer;
//	//	sendBuffer.SetOperationType(operationType);
//	//}
//}

//int Session::Accept(SOCKET& listenSocket)
//{
//	Init(eOperationType::Accept);
//	
//	if (::AcceptEx(listenSocket, GetSocket(), &recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &receiveBytes, &recvBuffer))
//	{
//		printf("���� Ŭ���� ���� - AcceptEx ���� (%d)\n", WSAGetLastError());
//		return 1;
//	}
//
//	return 0;
//}

int Session::Recv()
{
	DWORD flags = 0;
	
	recvBuffer->Init();

	// WSARecv(����, wsa����, wsa���۰���, ���ŵȵ������� ����Ʈ�� ���� ��, �÷���, ��������)
	if (::WSARecv(GetSocket(), &recvBuffer->wsaBuffer, 1, &receiveBytes, &flags, recvBuffer, NULL) == SOCKET_ERROR)
	{
		if (::WSAGetLastError() != ERROR_IO_PENDING)
		{
			printf("���� Ŭ���� ���� - WSARecv ���� (%d)\n", WSAGetLastError());
			return 1;
		}
	}

	return 0;
}

int Session::Send(const char* data, int size)
{
	// RecvBuffer�� �޸� Send ������ ��û�� �� �� �ֱ⿡ �������� ����� Send ���� �̺�Ʈ �޴� �κп��� �����ش�.
	Buffer* sendBuffer = new Buffer(eOperationType::Send);
	DWORD sendBytes = 0;
	DWORD flags = 0;

	//ZeroMemory(sendBuffer, sizeof(Buffer));
	//ZeroMemory(sendBuffer->messageBuffer, MAX_BUFFER);
	//sendBuffer->wsaBuffer.buf = sendBuffer->messageBuffer;
	//sendBuffer->wsaBuffer.len = size;
	//sendBuffer->SetOperationType(eOperationType::Send);
	sendBuffer->wsaBuffer.len = size;
	memcpy(sendBuffer->messageBuffer, data, size);

	// WSASend(����, wsa����, wsa���۰���, �۽ŵȵ������� ����Ʈ�� ���� ��, �÷���, ��������)
	if (::WSASend(GetSocket(), &(sendBuffer->wsaBuffer), 1, &sendBytes, flags, sendBuffer, NULL) == SOCKET_ERROR)
	{
		if (::WSAGetLastError() != ERROR_IO_PENDING)
		{
			printf("���� Ŭ���� ���� - WSASend ���� (%d)\n", WSAGetLastError());
			return 1;
		}
	}

	return 0;
}
