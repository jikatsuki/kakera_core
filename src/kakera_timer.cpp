#include "kakera_timer.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_tools.hpp"
#include <map>

using namespace std;
using namespace kakera_private;

kakera_TimerID kakera_RegisterTimer(int interval, kakera_TimerCallback callback, void * userdata)
{
    TimerTable& timeTable = TimerTable::getInstance();
    TimerInfo* info = new TimerInfo;
    info->callback = callback;
    info->userdata = userdata;
    info->startTime = chrono::high_resolution_clock::now();
    int id =  SDL_AddTimer(interval, SDLTimerCallback, info);
    timeTable.table.emplace(id, info);
    return id;
}

void kakera_RegisterLimitedTimer(int duration, int interval, kakera_TimerCallback callback, void * userdata)
{
    TimerTable& timeTable = TimerTable::getInstance();
    TimerInfo* info = new TimerInfo;
    info->callback = callback;
    info->userdata = userdata;
    info->startTime = chrono::high_resolution_clock::now();
    info->isLimited = true;
    chrono::milliseconds totalLength(duration);
    info->endTime = info->startTime + totalLength;
    int id = SDL_AddTimer(interval, SDLTimerCallback, info);
    timeTable.table.emplace(id, info);
}

void kakera_DestroyTimer(kakera_TimerID id)
{
    TimerTable& timeTable = TimerTable::getInstance();
    auto iter = timeTable.table.find(id);
    if (iter != timeTable.table.end())
    {
        TimerInfo* info = iter->second;
        delete info;
        timeTable.table.erase(iter);
    }
}
