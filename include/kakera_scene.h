#ifndef KAKERA_CORE_SCENE
#define KAKERA_CORE_SCENE

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"

typedef void(*kakera_SceneEventCallback)(kakera_Scene*);

typedef enum {
    KAKERA_SCENE_ON_CREATE,
    KAKERA_SCENE_ON_DESTROY,
    KAKERA_SCENE_ON_START,
    KAKERA_SCENE_ON_STOP,
    KAKERA_SCENE_ON_RESTART,
    KAKERA_SCENE_ON_PAUSE,
    KAKERA_SCENE_ON_RESUME
} kakera_SceneEvents;

extern KAKERA_EXPORT kakera_Scene*      kakera_CreateScene(const char* name);
extern KAKERA_EXPORT void               kakera_DestroyScene(kakera_Scene* scene);
extern KAKERA_EXPORT const char*        kakera_GetSceneName(kakera_Scene* scene);
extern KAKERA_EXPORT void               kakera_AddElementToScene(kakera_Scene* scene, kakera_Element* element, kakera_Element* parent);
extern KAKERA_EXPORT kakera_Element*    kakera_GetElementByIDFromScene(kakera_Scene* scene, const char* id);
extern KAKERA_EXPORT void               kakera_DeleteElementFromScene(kakera_Scene* scene, kakera_Element* element);
extern KAKERA_EXPORT void               kakera_BindEventToScene(kakera_Scene* scene, kakera_SceneEvents event, kakera_SceneEventCallback callback);

#define kakera_SetRootElementForScene(scene,element) kakera_AddElementToScene(scene, element, NULL)

#ifdef __cplusplus
}
#endif

#endif //!KAKERA_CORE_SCENE