#pragma once
constexpr int MAX_BUFFER = 1024;

class Buffer : public OVERLAPPED
{
public:
	Buffer(eOperationType operationType);

	void Init();
	//~Buffer()
	//{
	//	//delete Pointer;
	//	//delete hEvent;
	//	delete[] messageBuffer;
	//};

	public:
		WSABUF wsaBuffer;
		char messageBuffer[MAX_BUFFER];
		eOperationType GetOperationType() { return mOperationType; }
	private:
		eOperationType mOperationType;
};
