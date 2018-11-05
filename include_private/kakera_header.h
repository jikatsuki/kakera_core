#ifndef KAKERA_CORE_HEADER
#define KAKERA_CORE_HEADER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>

#define kakera_RunCallback(target_ptr,type)\
{\
    auto iter = target_ptr->callbackList.find(type);\
    if (iter != target_ptr->callbackList.end())\
        iter->second(target_ptr);\
}

#define kakera_RunCallbackAsync(target_ptr,type,lock)\
{\
    auto target = target_ptr;\
    auto iter = target->callbackList.find(type);\
    if (iter != target->callbackList.end())\
    {\
        auto callback = iter->second;\
        std::thread temp_thread([&target, &callback, &lock]() {\
            std::lock_guard<std::mutex> locker(*lock);\
            callback(target);\
        });\
        temp_thread.detach();\
    }\
}

#define kakera_CheckNullPointer(ptr)\
{\
    if (ptr == nullptr)\
    {\
        fprintf(stderr, "NullPointerException");\
        abort();\
    }\
}

#define KAKERA_USING_REFRESH_EVENT extern Uint32 kakera_RefreshEvent;

#define kakera_PushRefreshEvent()\
{\
    SDL_Event refresh_event;\
    SDL_zero(refresh_event);\
    refresh_event.type = SDL_USEREVENT;\
    refresh_event.user.type = kakera_RefreshEvent;\
    SDL_PushEvent(&refresh_event);\
}

#endif //!KAKERA_CORE_HEADER