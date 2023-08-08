#include "pch.h"
#include "IOCP.h"
#include "Session.h"

//DWORD WINAPI makeThread(LPVOID hIOCP);

IOCP::IOCP() : mIOCP(NULL)
{
}

IOCP::~IOCP()
{
}

int IOCP::Init()
{
	mIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mIOCP == NULL) {
		printf("IOCP 클래스 에러 - IOCP 핸들 생성 실패");
		return 1;
	}

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	int threadCount = systemInfo.dwNumberOfProcessors * 2;

	for (int i = 0; i < threadCount; i++)
	{
		mThreadPool.push_back(new thread{ &IOCP::WorkerThread, this });
	}

	return 0;
}

void IOCP::Run(ListenSocket*& listenSocket)
{
	NetAddress clientAddr;
	SOCKET clientSocket;

	Session* session;

	int id = 1;

	while (1)
	{
		clientSocket = listenSocket->Accept(clientAddr);
		if (clientSocket == INVALID_SOCKET)
		{
			printf("IOCP 클래스 에러 - 리슨 소켓에서 억셉트 실패 (%d)\n", WSAGetLastError());
			return;
		}

		session = new Session(id++, clientSocket);
		mSessionList.insert({ id, session });

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(session->GetSocket()), mIOCP, reinterpret_cast<ULONG_PTR>(session), 0);

		int ReturnValue = session->Recv();

		if (ReturnValue != ERROR_SUCCESS)
		{
			ReturnValue = WSAGetLastError();

			if (ReturnValue != ERROR_IO_PENDING)
			{
				printf("IOCP 클래스 에러 - 수신 실패 (%d)\n", WSAGetLastError());
				return;
			}
		}
	}
}

void IOCP::WorkerThread()
{
	DWORD bytesTransferred;
	Session* session;
	Buffer* buffer;
	eOperationType operationType;
	while (true)
	{
		if (::GetQueuedCompletionStatus(mIOCP, &bytesTransferred, (PULONG_PTR)&session, (LPOVERLAPPED*)&buffer, INFINITE) == 0)
		{
			printf("IOCP 클래스 에러 - GetQueuedCompletionStatus 실패 (%d)\n", WSAGetLastError());
			delete session;
			continue;
		}

		operationType = buffer->GetOperationType();

		if (operationType == eOperationType::Recv)
		{
			if (bytesTransferred == 0)
			{
				delete session;
				continue;
			}

			printf("수신된 메세지: %s (%d bytes)\n", buffer->wsaBuffer.buf, bytesTransferred);

			session->Send(buffer->wsaBuffer.buf, bytesTransferred); // Echo

			session->Recv();
		}
		else if (operationType == eOperationType::Send)
		{
			printf("송신된 메세지: %s (%d bytes)\n", buffer->wsaBuffer.buf, buffer->wsaBuffer.len);
			delete buffer;
		}
		else 
		{
			printf("eOperationType::None\n");
		}
	}
}

void IOCP::AcceptThread() 
{
	//SOCKET clientSocket;

	//while (true)
	//{
	//	::AcceptEx(mListenSocket->GetSocket(), clientSocket)
	//}
}