#pragma once

#include <Windows.h>

class Utils
{
public:
	static DWORD GetModuleBase(char*);
	static DWORD GetModuleSize(char*);
	static DWORD FindPattern(char*, char*, DWORD, DWORD, DWORD);
};