#ifndef KAKERA_CORE_TOOLS
#define KAKERA_CORE_TOOLS

#include "kakera_header.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;

template<typename T>
inline T getAbsoluteValue(T num) noexcept
{
    if (num >= 0)
        return num;
    else
        return -num;
}

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

static vector<string> splitString(const string& str, const char separator)
{
    vector<string> result;
    stringstream strstream(str);
    string temp;
    while (getline(strstream, temp, separator))
    {
        result.emplace_back(temp);
    }
    return result;
}

#endif // !KAKERA_CORE_TOOLS
