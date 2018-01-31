#pragma once
#include "windef.h"
#include "SkyStruct.h"
#include <hal.h>
#include "VirtualMemoryManager.h"
#include "PhysicalMemoryManager.h"
#include "ProcessUtil.h"
#include "PIT.h"

void SKYASSERT(bool result, const char* pMsg);

/////////////////////////////////////////////////////////////////////////////
//동기화
/////////////////////////////////////////////////////////////////////////////
typedef struct _CRITICAL_SECTION {
	
	LONG LockRecursionCount;
	HANDLE OwningThread;        // from the thread's ClientId->UniqueThread
	      
} CRITICAL_SECTION, *LPCRITICAL_SECTION;;

extern CRITICAL_SECTION g_criticalSection;

void SKYAPI kEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void SKYAPI kInitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void SKYAPI kLeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

/////////////////////////////////////////////////////////////////////////////
//스레드
/////////////////////////////////////////////////////////////////////////////
DWORD SKYAPI kGetCurrentThreadId();

void GetLocalTime(LPSYSTEMTIME lpSystemTime);
BYTE SetLocalTime(LPSYSTEMTIME lpSystemTime);

void printf(const char* str, ...);
int kprintf(const char* str);
void ksleep(int millisecond);
char GetKeyboardInput();
HANDLE CreateThread(SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreateionFlags, LPDWORD lpThreadId);
void PauseSystem(const char* msg);

extern "C"
{
	void TerminateProcess();
	uint32_t MemoryAlloc(size_t size);
	void MemoryFree(void* p);
	void CreateDefaultHeap();
	void TerminateMemoryProcess();
}


