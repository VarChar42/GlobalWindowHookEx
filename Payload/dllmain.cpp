// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

std::string Narrow(const wchar_t* s, char defaultChar = '_')
{
    const std::locale& loc = std::locale();
    std::ostringstream stringStream;
    while (*s != L'\0') {
        stringStream << std::use_facet<std::ctype<wchar_t>>(loc).narrow(*s++, defaultChar);
    }
    return stringStream.str();
}

std::string GetLogFilePath(const char* name) {
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);

    std::string processName = Narrow(buffer);
    processName = processName.substr(processName.find_last_of("/\\") + 1);

    std::string path = "C:\\log\\";

    path += name;
    path += "-";
    path += processName;
    path += ".log";

    return path;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    std::string path = GetLogFilePath("main");

    FILE* file;
    fopen_s(&file, path.c_str(), "a+");

    if (file != NULL) {
        switch (ul_reason_for_call)
        {
        case DLL_PROCESS_ATTACH:
            fprintf(file, "DLL attach function called\n");
        case DLL_THREAD_ATTACH:
            fprintf(file, "DLL thread attach function called\n");
        case DLL_THREAD_DETACH:
            fprintf(file, "DLL thread detach function called\n");
        case DLL_PROCESS_DETACH:
            fprintf(file, "DLL detach function called\n");
            break;
        }

        fclose(file);
    }

    return TRUE;
}


extern "C" __declspec(dllexport) int handle(int code, WPARAM wParam, LPARAM lParam) {
  
    std::string path = GetLogFilePath("log");

    FILE* file;
    fopen_s(&file, path.c_str(), "a+");
  
    if (file != NULL) {
        fprintf(file, "wParam: 0x%08x\n", (int)wParam);
        fclose(file);
    }


    return(CallNextHookEx(NULL, code, wParam, lParam));
}

