#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

DWORD processId;

int main() {

	LPCSTR pathToDLL = "C:\\Users\\Niklas\\Documents\\Visual Studio 2017\\Projects\\Arma3H4ck\\Debug\\SimpleDll.dll";

	HWND hWnd = FindWindowA(0, ("Arma 3 Launcher"));

	GetWindowThreadProcessId(hWnd, &processId);
	if (!hWnd) {
		return -1;
	}

	HANDLE armaProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	LPVOID processDLLPath = VirtualAllocEx(armaProcess, 0, strlen(pathToDLL) + 1, MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(armaProcess, processDLLPath, (LPVOID)pathToDLL, strlen(pathToDLL) + 1, 0);

	HANDLE loadThread = CreateRemoteThread(armaProcess, 0, 0, 
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),("LoadLibraryA")), processDLLPath, 0, 0);

	WaitForSingleObject(loadThread, INFINITE);

	cout << "DLL path is allocated at: " << processDLLPath << " with path " << pathToDLL << endl;

	cin.get();

	VirtualFreeEx(armaProcess, processDLLPath, strlen(pathToDLL) + 1, MEM_RELEASE);

}