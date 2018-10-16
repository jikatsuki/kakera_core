#ifndef KAKERA_CORE_SCENE
#define KAKERA_CORE_SCENE

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"

typedef struct kakera_Scene;

extern KAKERA_EXPORT kakera_Scene* kakera_CreateScene();
extern KAKERA_EXPORT void kakera_DestroyScene(kakera_Scene* scene);

#ifdef __cplusplus
}
#endif

#endif //!KAKERA_CORE_SCENE