
#include "stdafx.h"
#include "../wow64ext.hpp"

#include <TlHelp32.h>

int main()
{
	X64Caller RtlCreateUserThread("RtlCreateUserThread");
	if (!RtlCreateUserThread) return 0;
	X64Caller LdrUnloadDll("LdrUnloadDll");
	if (!LdrUnloadDll) return 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	if (Process32First(hSnapshot, &pe32)) {
		do {
			// _tprintf_s(_T("%s [%u]\n"), pe32.szExeFile, pe32.th32ProcessID);
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			BOOL isRemoteWow64 = FALSE;
			IsWow64Process(hProcess, &isRemoteWow64);
			if (!isRemoteWow64) {
				DWORD64 wow64DllBaseAddr = GetModuleHandle64(hProcess, _T("x64.dll"));
				if (wow64DllBaseAddr) {
					DWORD64 ret = RtlCreateUserThread(hProcess, NULL, FALSE, 0, 0, NULL, LdrUnloadDll, wow64DllBaseAddr, NULL, NULL);
				}
			}
		} while (Process32Next(hSnapshot, &pe32));
	}
    return 0;
}

