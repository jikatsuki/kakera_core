#ifndef KAKERA_CORE_TOOLS
#define KAKERA_CORE_TOOLS

template<typename T>
T getAbsoluteValue(T num)
{
    if (num >= 0)
    {
        return num;
    }
    else
    {
        return -num;
    }
}

template<typename T>
T kakera_max(T a, T b)
{
    if (a >= b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

template<typename T>
T kakera_min(T a, T b)
{
    if (a <= b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

bool isPointInArea(int& x, int& y, SDL_Rect* area)
{
    if (x >= area->x && x <= (area->x + area->h) && y >= area->y && y <= (area->y + area->h))
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif // !KAKERA_CORE_TOOLS
