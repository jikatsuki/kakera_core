#include "kakera_tools.hpp"

KAKERA_PRIVATE_USING_REFRESH_EVENT

using namespace std;
using namespace kakera_private;

vector<string> kakera_private::splitString(const string& str, const char separator)
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

int kakera_private::FastSqrt(float number)
{
    //Thanks for John D. Carmack II & Chris Lomont
    long i;
    float x, y;
    const float f = 1.5f;

    x = number * 0.5f;
    y = number;
    i = *(long*)&y;
    i = 0x5f375a86 - (i >> 1);
    y = *(float*)&i;
    y = y * (f - (x * y * y));
    y = y * (f - (x * y * y));
    return int(number * y + 1);
}

void kakera_private::PushRefreshEvent()
{
    SDL_Event refresh_event;
    SDL_zero(refresh_event);
    refresh_event.type = SDL_USEREVENT;
    refresh_event.user.type = kakera_RefreshEvent;
    refresh_event.user.data1 = nullptr;
    SDL_PushEvent(&refresh_event);
}

void kakera_private::PushRefreshEvent(RefreshType type, RefreshInfo info)
{
    SDL_Event refresh_event;
    SDL_zero(refresh_event);
    refresh_event.type = SDL_USEREVENT;
    refresh_event.user.type = kakera_RefreshEvent;
    SDL_Rect* refreshArea = new SDL_Rect;
    switch (type)
    {
    case RefreshType::Refresh_Moved:
    {
        break;
    }
    case RefreshType::Refresh_Unmoved:
    {
        break;
    }
    case RefreshType::Refresh_Rotate:
    {
        break;
    }
    }
    refresh_event.user.data1 = refreshArea;
    SDL_PushEvent(&refresh_event);
}

bool kakera_private::Is2RectIntersected(SDL_Rect * rect1, SDL_Rect * rect2)
{
    int x1_1 = rect1->x, x2_1 = rect1->x + rect1->w, y1_1 = rect1->y, y2_1 = rect1->y + rect1->h;
    int x1_2 = rect2->x, x2_2 = rect2->x + rect2->w, y1_2 = rect2->y, y2_2 = rect2->y + rect2->h;
    if (abs(x2_2 + x1_2 - x2_1 - x1_1) <= (x2_1 - x1_1 + x2_2 - x1_2) &&
        abs(y2_2 + y1_2 - y2_1 - y1_1) <= (y2_1 - y1_1 + y2_2 - y1_2))
        return true;
    else
        return false;
}

SDL_Rect * kakera_private::Get2RectIntersection(SDL_Rect * rect1, SDL_Rect * rect2)
{
    int x1_1 = rect1->x, x2_1 = rect1->x + rect1->w, y1_1 = rect1->y, y2_1 = rect1->y + rect1->h;
    int x1_2 = rect2->x, x2_2 = rect2->x + rect2->w, y1_2 = rect2->y, y2_2 = rect2->y + rect2->h;
    int x1_r = max(x1_1, x1_2), y1_r = max(y1_1, y1_2), x2_r = min(x2_1, x2_2), y2_r = min(y2_1, y2_2);
    SDL_Rect* result = new SDL_Rect({
        x1_r,
        y1_r,
        x2_r - x1_r,
        y2_r - y1_r
    });
    return result;
}
