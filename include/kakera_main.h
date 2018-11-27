#ifndef KAKERA_CORE_MAIN
#define KAKERA_CORE_MAIN

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>

#define KAKERA_CORE_BUILDIN
#ifdef _MSC_VER
    //
    #ifdef KAKERA_CORE_BUILDIN
        #define KAKERA_EXPORT __declspec(dllexport)
    #else
        #define KAKERA_EXPORT __declspec(dllimport)
    #endif //!KAKERA_CORE_BUILDIN
#else
    #define KAKERA_EXPORT
#endif //!_MSC_VER

#ifdef _MSC_VER
    #define KAKERA_DEPRECATED __declspec(deprecated("THIS IS AN UNRECOMMENDED API."))
#elif __GNUC__
    #define KAKERA_DEPRECATED __attribute__((deprecated))
#else
    #define KAKERA_DEPRECATED
#endif

#define KAKERA_CORE_VERSION "1.1.0 Asterisk"

extern KAKERA_EXPORT void kakera_Initialize();
extern KAKERA_EXPORT void kakera_Terminate();
typedef unsigned int(*kakera_TimerCallback)(unsigned int, void*);
extern KAKERA_EXPORT int kakera_SetTimer(unsigned int interval, kakera_TimerCallback callback, void* userdata);
extern KAKERA_EXPORT void kakera_DestroyTimer(int timerID);

#ifdef __cplusplus
}
#endif

#endif //!KAKERA_CORE_MAIN