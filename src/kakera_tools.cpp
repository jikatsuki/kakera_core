#include "kakera_tools.hpp"

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
    SDL_Rect* result = new SDL_Rect({
        max(x1_1, x1_2),
        max(y1_1, y1_2),
        min(x2_1, x2_2) - max(x1_1, x1_2),
        min(y2_1, y2_2) - max(y1_1, y1_2)
    });
    return result;
}

SDL_Rect * kakera_private::Unite2Rect(SDL_Rect * rect1, SDL_Rect * rect2)
{
    int x1_1 = rect1->x, y1_1 = rect1->y;
    int x2_2 = rect2->x + rect2->w, y2_2 = rect2->y + rect2->h;
    SDL_Rect* result = new SDL_Rect;
    result->x = x1_1;
    result->y = y1_1;
    if (x2_2 - x1_1 < rect1->w)
        result->w = rect1->w;
    else
        result->w = x2_2 - x1_1;

    if (y2_2 - y1_1 < rect1->h)
        result->h = rect1->h;
    else
        result->h = y2_2 - y1_1;
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
    SDL_Rect* result = new SDL_Rect;
    result->x = rect->x;
    result->y = rect->y;
    result->w = rect->w;
    result->h = rect->h;
    return result;
}
