// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <codecvt>
#include <Windows.h>
#include <iostream>
#include <detours.h>
#include <locale>

#include "il2cpp-appdata.h"
#include "helpers.h"

using namespace app;

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"bypass_log.log";
const std::string NotMelonLoader = "totally_not_melon_loader";
const bool logging = false;

String* not_melon_loader;

void DoNothingMethod(MethodInfo* method)
{
	// LogWrite("DoNothing_1\n");
	return;
}

bool File_Exists_Hook(String* str, MethodInfo* method)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wideToNarrow;
	std::string skey = wideToNarrow.to_bytes(std::wstring((const wchar_t*)
		(&((Il2CppString*)str)->chars), ((Il2CppString*)str)->length));

	//char p[500];
	//sprintf_s(p, "Searching for file %s", skey.c_str());
	//LogWrite(p);

	if (skey.find("dll") != std::string::npos || skey.find(NotMelonLoader) != std::string::npos)
	{
		//LogWrite(" blocked\n");
		return false;
	}

	//LogWrite("\n");
	return File_Exists(str, method);
}

bool Directory_Exists_Hook(String* str, MethodInfo* method)
{

	std::wstring_convert<std::codecvt_utf8<wchar_t>> wideToNarrow;
	std::string skey = wideToNarrow.to_bytes(std::wstring((const wchar_t*)
		(&((Il2CppString*)str)->chars), ((Il2CppString*)str)->length));

	//char p[500];
	//sprintf_s(p, "Searching for directory %s", skey.c_str());
	//LogWrite(p);

	if (skey.find("MelonLoader") != std::string::npos || skey.find(NotMelonLoader) != std::string::npos)
	{
		//LogWrite(" blocked\n");
		return false;
	}

	//LogWrite("\n");
	return Directory_Exists(str, method);
}


void* TryGetModuleHandleHook(String* str, MethodInfo* method)
{

	std::wstring_convert<std::codecvt_utf8<wchar_t>> wideToNarrow;
	std::string skey = wideToNarrow.to_bytes(std::wstring((const wchar_t*)
		(&((Il2CppString*)str)->chars), ((Il2CppString*)str)->length));

	char buf[500]{};

	//sprintf_s(buf, "blocked module handle of %s\n", skey.c_str());
	//LogWrite(buf);
	return nullptr;
}


String* GetMelonLoaderSearchStrings(Byte__Array* theArray, bool b, MethodInfo* method)
{
	return not_melon_loader;
}

void Run()
{

	NewConsole();
	LogWrite("Starting bypass\n");

	not_melon_loader = (String*)il2cpp_string_new(NotMelonLoader.c_str());

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// This method checks if the following apps are running.
	// It gets all the system's process and compares their names with the ones below, if any matches it calls Application.Quit()
	// - cheatengine
	// - sharpmonoinjector
	// - fiddler
	// - dnspy
	// - ollydbg
	// NOTE: this seems to be buggy because before finding this I got Phasmophobia running with Cheat Engine in the background.
	DetourAttach(&(PVOID&)u091Au0924u091Fu0929u091Au091Eu0924u091Bu0921u0927u0921_u091Eu0925u0927u0922u091Au0925u0926u0921u0923u0922u091E, DoNothingMethod);

	// This method checks for the following files in the games data directory, on the first one found, it deletes it and calls Application.Quit()
	// - modconfig.cfg
	// - sharpmonoinjector.dll
	// - coolphobialauncher.exe
	// - loader.exe
	// - launcher.exe
	DetourAttach(&(PVOID&)u091Au0924u091Fu0929u091Au091Eu0924u091Bu0921u0927u0921_u0922u0926u0920u091Fu091Au0925u0928u091Cu0926u0925u0925, DoNothingMethod); // Common cheat finder


	// this method calls GetModuleHandle, it is only called by the game's code and is used to detect loaded dlls
	DetourAttach(&(PVOID&) u0928u0924u0922u0920u0927u091Bu0928u0929u0921u0921u0924_u0923u0926u0922u0920u0922u091Bu0922u091Fu0927u0929u0922, TryGetModuleHandleHook);

	// I haven't been able to find where exactly it looks for the dll files, so I'm just hooking File.Exists() and Directory.Exists(), and returning false manually when it looks for a dll

	DetourAttach(&(PVOID&) File_Exists, File_Exists_Hook);
	DetourAttach(&(PVOID&)Directory_Exists, Directory_Exists_Hook);

	// Hooks the check for MelonLoader strings

	DetourAttach(&(PVOID&) u091Eu0925u0924u091Bu0924u091Fu0920u091Au0925u0927u0920_9_u091Cu0925u0926u0920u0923u091Cu0920u091Bu091Du091Bu091B, GetMelonLoaderSearchStrings);
	
	// Hooks quit for debugging
	//DetourAttach(&(PVOID&) Application_Quit_1, DoNothingMethod);
	//DetourAttach(&(PVOID&) Application_Quit, DoNothingMethod);
	
	DetourTransactionCommit();
	LogWrite("Bypass hooked!\n");

}
