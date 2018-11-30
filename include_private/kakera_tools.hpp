#ifndef KAKERA_CORE_TOOLS
#define KAKERA_CORE_TOOLS

#include "kakera_header.h"
#include "kakera_declaration.h"
#include "kakera_structs.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <type_traits>
#include <map>
#include <iostream>

using namespace std;

namespace kakera_private
{
    inline bool isPointInArea(int& x, int& y, const SDL_Rect* area) noexcept
    {
        if (area != nullptr)
        {
            if (x >= area->x && x <= (area->x + area->w) && y >= area->y && y <= (area->y + area->h))
                return true;
            else
                return false;
        }
        else
            return false;
    }

    vector<string> splitString(const string& str, const char separator);

    bool Is2RectIntersected(SDL_Rect* rect1, SDL_Rect* rect2);

    SDL_Rect* Get2RectIntersection(SDL_Rect* rect1, SDL_Rect* rect2);

    SDL_Rect* Unite2Rect(SDL_Rect* rect1, SDL_Rect* rect2);

    template<typename T>
    void CheckNullPointer(T ptr)
    {
        if (is_pointer<T>::value)
        {
            if (ptr == nullptr)
            {
                fprintf(stderr, "NullPointerException");
                abort();
            }
        }
    }

    template<typename Target, typename Type>
    void RunCallback(Target target_ptr, Type type)
    {
        auto iter = target_ptr->callbackList.find(type);
        if (iter != target_ptr->callbackList.end())
        {
            iter->second(target_ptr);
        }
    }

    void printSDLRect(const SDL_Rect& rect);

    SDL_Rect* ConvertRect(const kakera_Rectangle* rect);

    inline bool ConvertBool(kakera_Boolean& boolean)
    {
        return boolean == KAKERA_TRUE ? true : false;
    }

    kakera_Pixels* ClipPixels(const kakera_Pixels* src, SDL_Rect* rect);

    class TimerTable
    {
    public:
        static TimerTable& getInstance()
        {
            static TimerTable _instance;
            return _instance;
        }

        map<int, TimerInfo*> table;
    };

    Uint32 SDLTimerCallback(Uint32 interval, void * param);
}

#endif // !KAKERA_CORE_TOOLS
