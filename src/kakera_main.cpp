#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //!_MSC_VER

#include "kakera_main.h"
#include "kakera_header.h"
#include "kakera_tools.hpp"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ShellScalingAPI.h>
#include <Versionhelpers.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#endif //!_WIN32

Uint32 kakera_RefreshEvent;

void kakera_Initialize()
{
#ifdef _WIN32
    SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
#endif //!_WIN32
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    kakera_RefreshEvent = SDL_RegisterEvents(1);
    //Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID);
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
}

void kakera_Terminate()
{
    //Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
