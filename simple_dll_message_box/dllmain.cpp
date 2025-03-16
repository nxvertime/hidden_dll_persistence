// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include <Windows.h>





BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        
        MessageBox(NULL, L"https://github.com/nxvertime", L"DLL LOADED", MB_ICONINFORMATION | MB_OK);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        MessageBox(NULL, L"DLL unloaded",L"DLL UNLOADED", MB_ICONINFORMATION | MB_OK);

        break;
    }
    return TRUE;
}

