#pragma once

class Engine

{
public:
	Engine();
	~Engine();


private:
	DWORD clientDll;
	DWORD engineDll;
	DWORD clientDllSize;
	DWORD engineDllSize;

};
