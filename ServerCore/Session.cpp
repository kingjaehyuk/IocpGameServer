#include "pch.h"
#include "Session.h"

Session::Session(int id) : SocketBase(INVALID_SOCKET), mId(id)
{
	Init();
	printf("���� ���� ID: %d\n", mId);
}

Session::Session(int id, SOCKET clientSocket) : SocketBase(clientSocket), mId(id)
{
	Init();
	printf("���� ���� ID: %d\n", mId);
}

void Session::Init(eOperationType operationType)
{
	if (operationType == eOperationType::Recv)
	{
		ZeroMemory(&recvBuffer, sizeof(Buffer));
		ZeroMemory(&recvBuffer.messageBuffer, MAX_BUFFER);
		recvBuffer.wsaBuffer.len = MAX_BUFFER;
		recvBuffer.wsaBuffer.buf = recvBuffer.messageBuffer;
		recvBuffer.SetOperationType(operationType);
	}
	//else if (operationType == eOperationType::Send)
	//{
	//	ZeroMemory(&sendBuffer, sizeof(Buffer));
	//	ZeroMemory(&sendBuffer.messageBuffer, MAX_BUFFER);
	//	sendBuffer.wsaBuffer.len = MAX_BUFFER;
	//	sendBuffer.wsaBuffer.buf = sendBuffer.messageBuffer;
	//	sendBuffer.SetOperationType(operationType);
	//}
}

bool Session::Recv()
{
	DWORD flags = 0;

	Init(eOperationType::Recv);
	
	// WSARecv(����, wsa����, wsa���۰���, ���ŵȵ������� ����Ʈ�� ���� ��, �÷���, ��������)
	if (::WSARecv(GetSocket(), &recvBuffer.wsaBuffer, 1, &receiveBytes, &flags, &recvBuffer, NULL) == SOCKET_ERROR)
	{
		if (::WSAGetLastError() != ERROR_IO_PENDING)
		{
			printf("���� Ŭ���� ���� - WSARecv ���� (%d)\n", WSAGetLastError());
		}
	}

	return true;
}

int Session::Send(const char* data, int size)
{
	// RecvBuffer�� �޸� Send ������ ��û�� �� �� �ֱ⿡ �������� ����� Send ���� �̺�Ʈ �޴� �κп��� �����ش�.
	Buffer* sendBuffer = new Buffer();
	DWORD sendBytes = 0;
	DWORD flags = 0;

	ZeroMemory(sendBuffer, sizeof(Buffer));
	ZeroMemory(sendBuffer->messageBuffer, MAX_BUFFER);
	sendBuffer->wsaBuffer.buf = sendBuffer->messageBuffer;
	sendBuffer->wsaBuffer.len = size;
	sendBuffer->SetOperationType(eOperationType::Send);
	memcpy(sendBuffer->messageBuffer, data, size);

	// WSASend(����, wsa����, wsa���۰���, �۽ŵȵ������� ����Ʈ�� ���� ��, �÷���, ��������)
	if (::WSASend(GetSocket(), &(sendBuffer->wsaBuffer), 1, &sendBytes, flags, sendBuffer, NULL) == SOCKET_ERROR)
	{
		if (::WSAGetLastError() != ERROR_IO_PENDING)
		{
			printf("���� Ŭ���� ���� - WSASend ���� (%d)\n", WSAGetLastError());
		}
	}

	return 0;
}
