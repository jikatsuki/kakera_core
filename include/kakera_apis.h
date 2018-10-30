#ifndef KAKERA_CORE_APIS
#define KAKERA_CORE_APIS

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"
#include "kakera_window.h"
#include "kakera_scene.h"
#include "kakera_element.h"
#include "kakera_file.h"
#include "kakera_event.h"

#define kakera_SetRootElementForScene(scene,element) kakera_AddElementToScene(scene, element, NULL)
#define kakera_InitailizeElement(element,name) kakera_InitailizeElementComplex(element, KAKERA_ELEMENT_TYPE_STATIC, KAKERA_POSREFER_PARENT, name)
#define kakera_GetWindowFromElement(element) kakera_GetWindowFromScene(kakera_GetSceneFromElement(element))

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_APIS
