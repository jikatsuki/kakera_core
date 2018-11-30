#ifndef KAKERA_CORE_TIMER
#define KAKERA_CORE_TIMER

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"

typedef int(*kakera_TimerCallback)(void*);
typedef int kakera_TimerID;

extern KAKERA_EXPORT kakera_TimerID kakera_RegisterTimer(int interval, kakera_TimerCallback callback, void* userdata);
extern KAKERA_EXPORT void kakera_RegisterLimitedTimer(int duration, int interval, kakera_TimerCallback callback, void* userdata);
extern KAKERA_EXPORT void kakera_DestroyTimer(kakera_TimerID id);

#ifdef __cplusplus
}
#endif


#endif // !KAKERA_CORE_TIMER
