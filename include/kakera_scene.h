#ifndef KAKERA_CORE_SCENE
#define KAKERA_CORE_SCENE

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"
#include "kakera_event.h"

extern KAKERA_EXPORT kakera_Scene*      kakera_CreateScene();
extern KAKERA_EXPORT void               kakera_InitializeScene(kakera_Scene* scene);
extern KAKERA_EXPORT void               kakera_DestroyScene(kakera_Scene** scene);
extern KAKERA_EXPORT void               kakera_BindSceneWithWindow(kakera_Scene* scene, kakera_Window* window);
extern KAKERA_EXPORT void               kakera_AddElementToScene(kakera_Scene* scene, kakera_Element* element, kakera_Element* parent);
extern KAKERA_EXPORT kakera_Element*    kakera_GetElementByNameFromScene(kakera_Scene* scene, const char* name);
extern KAKERA_EXPORT void               kakera_DeleteElementFromScene(kakera_Scene* scene, kakera_Element* element);
extern KAKERA_EXPORT void               kakera_BindEventToScene(kakera_Scene* scene, kakera_SceneEvents event, kakera_SceneEventCallback callback);
extern KAKERA_EXPORT void               kakera_SetFocusElement(kakera_Scene* scene, kakera_Element* element);
extern KAKERA_EXPORT kakera_Window*     kakera_GetWindowFromScene(kakera_Scene* scene);
extern KAKERA_EXPORT void*              kakera_GetSceneUserdata(kakera_Scene* scene);
extern KAKERA_EXPORT void               kakera_StartScene(kakera_Scene* scene, void* userdata);

#ifdef __cplusplus
}
#endif

#endif //!KAKERA_CORE_SCENE