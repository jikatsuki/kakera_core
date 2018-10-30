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
    #define KAKERA_DEPRECATED __declspec(deprecated("WARNING: THIS IS AN UNRECOMMENDED API. YOU'D BETTER KNOW WHAT YOU ARE DOING."))
#elif __GNUC__
    #define KAKERA_DEPRECATED __attribute__((deprecated))
#else
    #define KAKERA_DEPRECATED
#endif

#define KAKERA_CORE_VERSION "0.9.2"

extern KAKERA_EXPORT void kakera_Initialize();
extern KAKERA_EXPORT void kakera_Terminate();

#ifdef __cplusplus
}
#endif

#endif //!KAKERA_CORE_MAIN