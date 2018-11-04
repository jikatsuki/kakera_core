#ifndef KAKERA_CORE_HEADER
#define KAKERA_CORE_HEADER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>

#define kakera_RunCallback(target_ptr,type)\
{\
    if (target_ptr->callbackList.find(type) != target_ptr->callbackList.end())\
        target_ptr->callbackList[type](target_ptr);\
}

#define kakera_RunCallbackAsync(target_ptr,type,lock)\
{\
    auto target = target_ptr;\
    std::thread temp_thread([&target, &lock]() {\
        auto iter = target->callbackList.find(type);\
        kakera_RunCallback(target,type);\
    });\
    temp_thread.detach();\
}

#define kakera_CheckNullPointer(ptr)\
{\
    if (ptr == nullptr)\
    {\
        fprintf(stderr, "NullPointerException");\
        abort();\
    }\
}

#endif //!KAKERA_CORE_HEADER