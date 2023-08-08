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

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)( // Callback 함수의 원형
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

// Unhandled Exception 발생시 호출될 콜백 함수
LONG WINAPI MiniDump::UnhandledExceptionFilter(struct _EXCEPTOIN_POINTERS* exceptionInfo)
{
	// 반드시 로딩
	HMODULE DllHandle = LoadLibrary(_T("DBGHELP.DLL"));
	if (DllHandle == NULL)
	{
		PrintLastError();
		return EXCEPTION_CONTINUE_SEARCH;
	}

	// DBGHELP.DLL에서 MiniDumpWriteDump를 불러와 Dump라고 정의하며 이걸로 덤프 파일을 생성합니다.
	MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");
	if (Dump == NULL)
	{
		PrintLastError();
		return EXCEPTION_CONTINUE_SEARCH;
	}

	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime); //현재시간 획득

	TCHAR DumpPath[MAX_PATH] = { 0, };

	// 덤프 파일 이름 설정
	_sntprintf_s(DumpPath, MAX_PATH, _TRUNCATE, _T("%d-%d-%d %d_%d_%d.dmp"),
		SystemTime.wYear,
		SystemTime.wMonth,
		SystemTime.wDay,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond);

	// 덤프 파일 생성
	HANDLE FileHandle = CreateFile(DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		PrintLastError();
		return EXCEPTION_CONTINUE_SEARCH;
	}

	// MiniDump 예외 정보 저장 구조체
	_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;
	MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
	MiniDumpExceptionInfo.ExceptionPointers = (PEXCEPTION_POINTERS)exceptionInfo;
	MiniDumpExceptionInfo.ClientPointers = NULL;

	// 현재 프로세스에 대한 덤프 기록을 실행합니다.
	BOOL Success = Dump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		FileHandle,    // 덤프를 기록할 파일 핸들
		MiniDumpNormal,
		&MiniDumpExceptionInfo, // MiniDump 예외 정보
		NULL,
		NULL);

	// 덤프 기록 성공시 수행
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
