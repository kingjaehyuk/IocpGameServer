#pragma once
class MiniDump
{
public:
	//static void PrintLastError(DWORD error = GetLastError());
	static LONG WINAPI UnhandledExceptionFilter(struct _EXCEPTOIN_POINTERS* exceptionInfo);
	static bool Begin(void);
	static bool End(void);
};