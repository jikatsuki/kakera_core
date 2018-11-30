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
    #define KAKERA_EXPERIMENTAL __declspec(deprecated("THIS IS AN EXPERIMENTAL API. MAYBE UNSTABLE."))
#elif __GNUC__
    #define KAKERA_DEPRECATED __attribute__((deprecated))
    #define KAKERA_EXPERIMENTAL __attribute__((deprecated))
#else
    #define KAKERA_DEPRECATED
    #define KAKERA_EXPERIMENTAL
#endif

#define KAKERA_CORE_VERSION "1.1.0 Asterisk"

extern KAKERA_EXPORT void kakera_Initialize();
extern KAKERA_EXPORT void kakera_Terminate();

#ifdef __cplusplus
}
#endif

#endif //!KAKERA_CORE_MAIN