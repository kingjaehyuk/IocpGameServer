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
	printf("세션 생성 ID: %d\n", mId);
}

Session::Session(Server& server, int id, SOCKET clientSocket) : SocketBase(clientSocket), mId(id), mServer(server)
{
	recvBuffer = new Buffer(eOperationType::Recv);
	printf("세션 생성 ID: %d\n", mId);
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
//		printf("세션 클래스 에러 - AcceptEx 실패 (%d)\n", WSAGetLastError());
//		return 1;
//	}
//
//	return 0;
//}

int Session::Recv()
{
	DWORD flags = 0;
	
	recvBuffer->Init();

	// WSARecv(소켓, wsa버퍼, wsa버퍼갯수, 수신된데이터의 바이트를 담을 값, 플래그, 오버랩드)
	if (::WSARecv(GetSocket(), &recvBuffer->wsaBuffer, 1, &receiveBytes, &flags, recvBuffer, NULL) == SOCKET_ERROR)
	{
		if (::WSAGetLastError() != ERROR_IO_PENDING)
		{
			printf("세션 클래스 에러 - WSARecv 실패 (%d)\n", WSAGetLastError());
			return 1;
		}
	}

	return 0;
}

int Session::Send(const char* data, int size)
{
	// RecvBuffer와 달리 Send 여러번 요청이 올 수 있기에 동적으로 만들고 Send 성공 이벤트 받는 부분에서 지워준다.
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

	// WSASend(소켓, wsa버퍼, wsa버퍼갯수, 송신된데이터의 바이트를 담을 값, 플래그, 오버랩드)
	if (::WSASend(GetSocket(), &(sendBuffer->wsaBuffer), 1, &sendBytes, flags, sendBuffer, NULL) == SOCKET_ERROR)
	{
		if (::WSAGetLastError() != ERROR_IO_PENDING)
		{
			printf("세션 클래스 에러 - WSASend 실패 (%d)\n", WSAGetLastError());
			return 1;
		}
	}

	return 0;
}
