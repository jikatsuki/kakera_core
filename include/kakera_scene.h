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
    KAKERA_SCENE_ON_STOP
} kakera_SceneEvents;

extern KAKERA_EXPORT kakera_Scene*      kakera_CreateScene();
extern KAKERA_EXPORT void               kakera_InitializeScene(kakera_Scene* scene);
extern KAKERA_EXPORT void               kakera_DestroyScene(kakera_Scene* scene);
extern KAKERA_EXPORT void               kakera_BindSceneWithWindow(kakera_Scene* scene, kakera_Window* window);
extern KAKERA_EXPORT void               kakera_AddElementToScene(kakera_Scene* scene, kakera_Element* element, kakera_Element* parent);
extern KAKERA_EXPORT kakera_Element*    kakera_GetElementByNameFromScene(kakera_Scene* scene, const char* name);
extern KAKERA_EXPORT void               kakera_DeleteElementFromScene(kakera_Scene* scene, kakera_Element* element);
extern KAKERA_EXPORT void               kakera_BindEventToScene(kakera_Scene* scene, kakera_SceneEvents event, kakera_SceneEventCallback callback);
extern KAKERA_EXPORT void               kakera_StartScene(kakera_Window* window, kakera_Scene* scene);

#define kakera_SetRootElementForScene(scene,element) kakera_AddElementToScene(scene, element, NULL)

#ifdef __cplusplus
}
#endif

#endif //!KAKERA_CORE_SCENE