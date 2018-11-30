#ifndef KAKERA_CORE_PRIVATE_APIS
#define KAKERA_CORE_PRIVATE_APIS

#include "kakera_header.h"
#include "kakera_declaration.h"
#include "kakera_part_implementation.h"
#include "kakera_timer.h"
#include "kakera_structs.hpp"
#include "kakera_tools.hpp"

using namespace std;

namespace kakera_private
{
    struct Refresh_Moved
    {
        SDL_Rect oldPosition;
        SDL_Rect newPosition;
    };

    struct Refresh_Unmoved
    {
        SDL_Rect refreshArea;
    };

    struct Refresh_Rotate
    {
        SDL_Rect positionAndSize;
    };

    union RefreshInfo
    {
        //この歌声がMILLION!!
        Refresh_Moved moved;
        Refresh_Unmoved unmoved;
        Refresh_Rotate rotate;
    };

    enum class RefreshType
    {
        Refresh_Moved,
        Refresh_Unmoved,
        Refresh_Rotate
    };

    void PushRefreshEvent(kakera_Scene* scene);

    void PushRefreshEvent(kakera_Scene* scene, RefreshType type, RefreshInfo& info);

    void DestroyElementWithoutRefreshingList(kakera_Element ** element);

    void RefreshAll(kakera_Window* window);

    void RefreshRect(kakera_Window* window, SDL_Rect* rect);

    void DeletePixels(kakera_Pixels** pixels);

    map<int, TimerInfo*>::iterator DestroyTimer(int id);
}

#endif //!KAKERA_CORE_PRIVATE_APIS