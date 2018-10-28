#ifndef KAKERA_CORE_TOOLS
#define KAKERA_CORE_TOOLS

#include "kakera_header.h"

template<typename T>
inline T getAbsoluteValue(T num) noexcept
{
    if (num >= 0)
        return num;
    else
        return -num;
}

inline bool isPointInArea(int& x, int& y, SDL_Rect* area) noexcept
{
    if (area != nullptr)
    {
        if (x >= area->x && x <= (area->x + area->w) && y >= area->y && y <= (area->y + area->h))
            return true;
        else
            return false;
    }
}

#endif // !KAKERA_CORE_TOOLS
