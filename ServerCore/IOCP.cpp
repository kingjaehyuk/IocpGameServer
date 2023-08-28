#include "pch.h"
#include "IOCP.h"
#include "Server.h"

IOCP::IOCP() : mIocpHandle(NULL)
{
}

IOCP::~IOCP()
{
}

int IOCP::Init()
{
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mIocpHandle == NULL) {
		printf("IOCP Ŭ���� ���� - IOCP �ڵ� ���� ����");
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

void IOCP::Add(Session& session)
{
	assert(mIocpHandle != nullptr);
	::CreateIoCompletionPort(reinterpret_cast<HANDLE>(session.GetSocket()), mIocpHandle, reinterpret_cast<ULONG_PTR>(&session), 0);
}

void IOCP::Add(ListenSocket& listenSocket)
{
	assert(mIocpHandle != nullptr);
	::CreateIoCompletionPort(reinterpret_cast<HANDLE>(listenSocket.GetSocket()), mIocpHandle, reinterpret_cast<ULONG_PTR>(&listenSocket), 0);
}

void IOCP::Run()
{
	for (int i = 0; i < mThreadPool.size(); i++)
	{
		mThreadPool[i]->join();
	}
	//NetAddress clientAddr;
	//SOCKET clientSocket;

	//Session* session;

	//int id = 1;

	//while (1)
	//{
	//	clientSocket = listenSocket->Accept(clientAddr);
	//	if (clientSocket == INVALID_SOCKET)
	//	{
	//		printf("IOCP Ŭ���� ���� - ���� ���Ͽ��� ���Ʈ ���� (%d)\n", WSAGetLastError());
	//		return;
	//	}

	//	session = new Session(id++, clientSocket);
	//	//mSessionList.insert({ id, session });

	//	CreateIoCompletionPort(reinterpret_cast<HANDLE>(session->GetSocket()), mIOCP, reinterpret_cast<ULONG_PTR>(session), 0);

	//	int ReturnValue = session->Recv();

	//	if (ReturnValue != ERROR_SUCCESS)
	//	{
	//		ReturnValue = WSAGetLastError();

	//		if (ReturnValue != ERROR_IO_PENDING)
	//		{
	//			printf("IOCP Ŭ���� ���� - ���� ���� (%d)\n", WSAGetLastError());
	//			return;
	//		}
	//	}
	//}
}

void IOCP::WorkerThread()
{
	DWORD bytesTransferred;
	Session* session;
	//Buffer* buffer;
	IocpEvent* iocpEvent;
	eIocpEventType eventType;
	while (true)
	{
		if (::GetQueuedCompletionStatus(mIocpHandle, &bytesTransferred, (PULONG_PTR)&session, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), INFINITE) == 0)
		{
			printf("IOCP Ŭ���� ���� - GetQueuedCompletionStatus ���� (%d)\n", WSAGetLastError());
			delete session;
			continue;
		}

		//eventType = buffer->GetEventType();
		eventType = iocpEvent->GetEventType();

		if (eventType == eIocpEventType::Recv)
		{
			if (bytesTransferred == 0)
			{
				delete session;
				continue;
			}

			//printf("[%d] ���ŵ� �޼���: %s (%d bytes)\n", session->GetId(), buffer->wsaBuffer.buf, bytesTransferred);

			session->HandleRecv(bytesTransferred);

			//session->Send(buffer->wsaBuffer.buf, bytesTransferred); // Echo

		}
		else if (eventType == eIocpEventType::Send)
		{
			//printf("[%d] �۽ŵ� �޼���: %s (%d bytes)\n", session->GetId(), buffer->wsaBuffer.buf, buffer->wsaBuffer.len);
			printf("[%d] �۽ŵ� �޼���: %s (%d bytes)\n", session->GetId(), static_cast<SendEvent*>(iocpEvent)->Buffer, sizeof(static_cast<SendEvent*>(iocpEvent)->Buffer) / sizeof(char));
			
			delete iocpEvent;
		}
		else if (eventType == eIocpEventType::Accept)
		{
			ListenSocket* listenSocket = reinterpret_cast<ListenSocket*>(session);
			listenSocket->HandleAccept();

			printf("���Ʈ ����\n");
		}
		else
		{
			printf("eOperationType::None\n");
		}
	}
}