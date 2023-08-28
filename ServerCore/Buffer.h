#pragma once
#include "RingBuffer.h"

constexpr int MAX_BUFFER = 1024;

class Buffer : public OVERLAPPED
{
public:
	Buffer(eIocpEventType eventType);

	void Init();
	//~Buffer()
	//{
	//	//delete Pointer;
	//	//delete hEvent;
	//	delete[] messageBuffer;
	//};

	public:
		WSABUF wsaBuffer;
		//char messageBuffer[MAX_BUFFER];
		RingBuffer* messageBuffer;
		eIocpEventType GetEventType() { return mEventType; }
	private:
		eIocpEventType mEventType;
};
