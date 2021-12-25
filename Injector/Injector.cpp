// Injector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

int main()
{
    printf("Loading payload dll...\n");

    HMODULE dll = LoadLibrary(L"Payload.dll");

    if (dll == NULL) {
        printf("Could not load dll .. Aborting!\n");
        return 0;
    }


    printf("Getting HOOKPROC...\n");
    HOOKPROC hookProc = (HOOKPROC)GetProcAddress(dll, "handle");

    if (hookProc == NULL) {
        printf("Could not load HOOKPROC .. Aborting!\n");
        return 0;
    }

    printf("Injection hook into ALL active processes...\n");
    HHOOK handle = SetWindowsHookEx(WH_GETMESSAGE, hookProc, dll, 0);

    if (handle == NULL) {
        printf("Could not get handle .. Aborting!\n");
        return 0;
    }

    printf("Press enter to unhook...\n");
    getchar();

    printf("Unhooking...\n");
    UnhookWindowsHookEx(handle);
    printf("Done");
}

