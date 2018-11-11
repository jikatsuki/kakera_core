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

void kakera_private::PushRefreshEvent()
{
    SDL_Event refresh_event;
    SDL_zero(refresh_event);
    refresh_event.type = SDL_USEREVENT;
    refresh_event.user.type = kakera_RefreshEvent;
    refresh_event.user.data1 = nullptr;
    SDL_PushEvent(&refresh_event);
}

void kakera_private::PushRefreshEvent(RefreshType type, RefreshInfo& info)
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
        int x1_1 = info.moved.oldPosition.x, y1_1 = info.moved.oldPosition.y;
        int x2_2 = info.moved.newPosition.x + info.moved.newPosition.w, y2_2 = info.moved.newPosition.y + info.moved.newPosition.h;
        refreshArea->x = x1_1;
        refreshArea->y = y1_1;
        if (x2_2 - x1_1 < info.moved.oldPosition.w)
        {
            refreshArea->w = info.moved.oldPosition.w;
        }
        else
        {
            refreshArea->w = x2_2 - x1_1;
        }

        if (y2_2 - y1_1 < info.moved.oldPosition.h)
        {
            refreshArea->h = info.moved.oldPosition.h;
        }
        else
        {
            refreshArea->h = y2_2 - y1_1;
        }
        break;
    }
    case RefreshType::Refresh_Unmoved:
    {
        refreshArea->x = info.unmoved.refreshArea.x;
        refreshArea->y = info.unmoved.refreshArea.y;
        refreshArea->w = info.unmoved.refreshArea.w;
        refreshArea->h = info.unmoved.refreshArea.h;
        break;
    }
    case RefreshType::Refresh_Rotate:
    {
        int center_x = info.rotate.positionAndSize.x + static_cast<float>(info.rotate.positionAndSize.w) / 2.0f;
        int center_y = info.rotate.positionAndSize.y + static_cast<float>(info.rotate.positionAndSize.h) / 2.0f;
        float d = sqrt(info.rotate.positionAndSize.w * info.rotate.positionAndSize.w + info.rotate.positionAndSize.h * info.rotate.positionAndSize.h);
        refreshArea->x = center_x - d / 2.0f <= 0 ? 0 : static_cast<int>(center_x - d / 2.0f);
        refreshArea->y = center_y - d / 2.0f <= 0 ? 0 : static_cast<int>(center_y - d / 2.0f);
        refreshArea->w = static_cast<int>(d);
        refreshArea->h = static_cast<int>(d);
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

void kakera_private::printSDLRect(const SDL_Rect & rect)
{
    cout << "Rect x=" << rect.x << endl;
    cout << "Rect y=" << rect.y << endl;
    cout << "Rect w=" << rect.w << endl;
    cout << "Rect h=" << rect.h << endl;
    cout << endl;
}

SDL_Rect * kakera_private::ConvertRect(const kakera_Rectangle * rect)
{
    SDL_Rect* result;
    result->x = rect->x;
    result->y = rect->y;
    result->w = rect->w;
    result->h = rect->h;
    return result;
}
