#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		MessageBox(0, L"Hey!", L"Hello", MB_ICONINFORMATION);
	}

	return 1;
}