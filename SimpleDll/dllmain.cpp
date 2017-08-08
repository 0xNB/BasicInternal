// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "stdafx.h"
#include "dllmain.h"
#include <stdio.h>

void SetupConsole();
DWORD WINAPI Loop(PVOID processThreadParameter);
HANDLE tLoop;

void SetupConsole() {
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);
	freopen("CONIN$", "rb", stdin);
	SetConsoleTitle("CSGOSimpleInternal");
}

DWORD WINAPI Loop(PVOID processThreadParameter) {
	printf("[STARTUP] Waiting for Arma Modules to load");
	while (!GetModuleHandle("") || !GetModuleHandle("")) {
		Sleep(500);
	}
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetupConsole();
		tLoop = CreateThread(0, 0, Loop, 0, 0, NULL);
		printf("[INFO] Dll succesfully attached to target process.\n");
		printf("[INFO] Press F10 to terminate.\n");
		if (tLoop) {
			printf("[LOG] Thread started.\n");
		}
		else {
			printf("[ERROR] Thread could not be started\n");
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: if (tLoop) {
		CloseHandle(tLoop);
	}
		break;
	}
	return TRUE;
}

