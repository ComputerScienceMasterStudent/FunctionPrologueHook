#include <iostream>
#include <Windows.h>

SIZE_T  bytesWritten = 0;
char    functionPrologue[6] = {};
FARPROC prevFuncAddr = NULL;

void __stdcall OutputDebugStringWHook(LPCWSTR lpOutputString)
{
	std::cout << "The output debug string is: " << lpOutputString;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)prevFuncAddr, functionPrologue, sizeof(functionPrologue), &bytesWritten);
	return OutputDebugStringW(lpOutputString);
}

int main()
{
	HINSTANCE library = LoadLibrary(L"kernel32.dll");
	SIZE_T    bytesRead = 0;
	if (library)
	{
		prevFuncAddr = GetProcAddress(library, "OutputDebugStringW");
		ReadProcessMemory(GetCurrentProcess(), prevFuncAddr, functionPrologue, 6, &bytesRead);
		void* newFunction = &OutputDebugStringWHook;
		char  hook[6] = {104,0,0,0,0,195};
		memcpy_s(hook + 1, 4, &newFunction, 4);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)prevFuncAddr, hook, sizeof(hook), &bytesWritten);
		OutputDebugString(L"test");
	}
	return 0;
}
