#include "pch.h"
#include "MiniDump.h"
#include "MiniDump.h"
#include <iostream>
#include <tchar.h>
#include <DbgHelp.h>

void PrintLastError(DWORD error = GetLastError())
{
	TCHAR* lpOSMsg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpOSMsg,
		0,
		NULL);
	_tprintf(_T("[ERROR][% d] % s\n"), error, lpOSMsg);
	// std::cout << "[ERROR] [" << error << "] " << lpOSMsg << std::endl;
	LocalFree(lpOSMsg);
}

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL;

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)( // Callback �Լ��� ����
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

// Unhandled Exception �߻��� ȣ��� �ݹ� �Լ�
LONG WINAPI MiniDump::UnhandledExceptionFilter(struct _EXCEPTOIN_POINTERS* exceptionInfo)
{
	// �ݵ�� �ε�
	HMODULE DllHandle = LoadLibrary(_T("DBGHELP.DLL"));
	if (DllHandle == NULL)
	{
		PrintLastError();
		return EXCEPTION_CONTINUE_SEARCH;
	}

	// DBGHELP.DLL���� MiniDumpWriteDump�� �ҷ��� Dump��� �����ϸ� �̰ɷ� ���� ������ �����մϴ�.
	MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");
	if (Dump == NULL)
	{
		PrintLastError();
		return EXCEPTION_CONTINUE_SEARCH;
	}

	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime); //����ð� ȹ��

	TCHAR DumpPath[MAX_PATH] = { 0, };

	// ���� ���� �̸� ����
	_sntprintf_s(DumpPath, MAX_PATH, _TRUNCATE, _T("%d-%d-%d %d_%d_%d.dmp"),
		SystemTime.wYear,
		SystemTime.wMonth,
		SystemTime.wDay,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond);

	// ���� ���� ����
	HANDLE FileHandle = CreateFile(DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		PrintLastError();
		return EXCEPTION_CONTINUE_SEARCH;
	}

	// MiniDump ���� ���� ���� ����ü
	_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;
	MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
	MiniDumpExceptionInfo.ExceptionPointers = (PEXCEPTION_POINTERS)exceptionInfo;
	MiniDumpExceptionInfo.ClientPointers = NULL;

	// ���� ���μ����� ���� ���� ����� �����մϴ�.
	BOOL Success = Dump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		FileHandle,    // ������ ����� ���� �ڵ�
		MiniDumpNormal,
		&MiniDumpExceptionInfo, // MiniDump ���� ����
		NULL,
		NULL);

	// ���� ��� ������ ����
	if (Success)
	{
		CloseHandle(FileHandle);
		return EXCEPTION_EXECUTE_HANDLER;
	}
	CloseHandle(FileHandle);

	return EXCEPTION_CONTINUE_SEARCH;
}

bool MiniDump::Begin(void)
{
	SetErrorMode(SEM_FAILCRITICALERRORS);

	PreviousExceptionFilter = SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)UnhandledExceptionFilter);

	return true;
}

bool MiniDump::End(void)
{
	SetUnhandledExceptionFilter(PreviousExceptionFilter);

	return true;
}
