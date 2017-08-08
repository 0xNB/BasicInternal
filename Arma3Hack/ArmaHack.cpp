#include <iostream>
#include <Windows.h>
#include <string>
#include <TlHelp32.h>

#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif


using namespace std;
bool InjectDll(DWORD ProcessId);

DWORD processId;
char ProcessName[] = "arma3.exe";
std::string DllName = "SimpleDll.dll";
LPCSTR pathToDLL;
typedef HINSTANCE(*fpLoadLibrary)(char*);

int main() {

	processId = NULL;
	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
	HANDLE hProc;

	// Getting relative path of our our injector exe and creating the corresponding dll path

	TCHAR bufCurrentDirectory[MAX_PATH + 1] = { 0 };
	DWORD dwNumCharacters = ::GetModuleFileName(NULL, bufCurrentDirectory, MAX_PATH);
	String arr_w(bufCurrentDirectory);
	std::string arr_s(arr_w.begin(), arr_w.end() );
	arr_s = arr_s.substr(0, arr_s.find_last_of("\\/")).append("\\").append(DllName);
	pathToDLL = arr_s.c_str();


	if (dwNumCharacters == 0) {
		return -1;
	}
	else {
		cout << dwNumCharacters << "  " << arr_s << endl;
	}

	while (!processId) {
		if (GetAsyncKeyState(VK_F10)) {
			return -1;
		}
		system("CLS");
		cout << "Searching for " << ProcessName << "..." << endl;
		cout << "Make sure the game is running" << endl;
		cout << "Press F10 to close search";
		hProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (Process32First(hProc, &pe32)) {
			do {
				if (!strcmp(pe32.szExeFile, ProcessName)) {
					processId = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(hProc, &pe32));
		}
		Sleep(1000);
	}

	while (!InjectDll(processId)) {
		system("CLS");
		cout << "DLL failed to inject" << endl;
		Sleep(1000);
	}

	cout << "DLL Injected successfuly!" << endl << endl;
	cout << "Closing Injector in 6 seconds" << endl;

	Sleep(6000);

	return 0;
}

bool InjectDll(DWORD processId) {

	HANDLE hProc;
	LPVOID processDLLPath;

	hProc = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	processDLLPath = VirtualAllocEx(hProc, 0, strlen(pathToDLL) + 1, MEM_COMMIT, PAGE_READWRITE);

	bool written = WriteProcessMemory(hProc, processDLLPath, (LPVOID)pathToDLL, strlen(pathToDLL) + 1, 0);

	HANDLE loadThread = CreateRemoteThread(hProc, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), ("LoadLibraryA")), processDLLPath, 0, 0);

	WaitForSingleObject(loadThread, INFINITE);

	cout << "DLL path is allocated at: " << processDLLPath << " with path " << pathToDLL << endl;

	VirtualFreeEx(hProc, processDLLPath, strlen(pathToDLL) + 1, MEM_RELEASE);
	CloseHandle(hProc);

	return written;
}