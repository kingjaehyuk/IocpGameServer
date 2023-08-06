#include "pch.h"
#include "IOCP.h"
#include "Session.h"

//DWORD WINAPI makeThread(LPVOID hIOCP);

IOCP::IOCP() : mListenSocket(nullptr), mIOCP(NULL)
{
}

IOCP::~IOCP()
{
}

bool IOCP::InitSocket()
{
	mListenSocket = new ListenSocket;

	if (mListenSocket->GetSocket() == INVALID_SOCKET)
	{
		shutdown("IOCP 클래스 에러 - 리슨 소켓 생성 실패");
		return false;
	}

	return true;
}

bool IOCP::BindAndListen(int port, int backlog)
{
	NetAddress serverAddr(port);

	if (mListenSocket->Bind(serverAddr) == SOCKET_ERROR)
	{
		shutdown("IOCP 클래스 에러 - 리슨 소켓 바인드 실패");
		return false;
	};

	if (mListenSocket->Listen(backlog) == SOCKET_ERROR)
	{
		shutdown("IOCP 클래스 에러 - 리슨 소켓 리슨 실패");
		return false;
	};

	return true;
}

bool IOCP::InitIocp()
{
	mIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mIOCP == NULL) {
		shutdown("IOCP 클래스 에러 - IOCP 핸들 생성 실패");
		return false;
	}

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	int threadCount = systemInfo.dwNumberOfProcessors * 2;

	for (int i = 0; i < threadCount; i++)
	{
		mThreadPool.push_back(new thread{ &IOCP::WorkerThread, this });
	}

	return true;
}

void IOCP::Run()
{
	NetAddress clientAddr;
	SOCKET clientSocket;

	Session* session;

	int id = 1;

	while (1)
	{
		clientSocket = mListenSocket->Accept(clientAddr);
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

void IOCP::shutdown(const char* message)
{
	cout << message << ' ' << WSAGetLastError() << endl;

	mListenSocket->Close();

	WSACleanup();
}

void IOCP::WorkerThread() {
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