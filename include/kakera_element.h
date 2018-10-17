#ifndef KAKERA_CORE_ELEMENT
#define KAKERA_CORE_ELEMENT

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"
#include <stdbool.h>

typedef enum {

} kakera_ElementEvents;

typedef void(*kakera_ElementEventCallback)(kakera_Element*);

extern KAKERA_EXPORT kakera_Element* kakera_CreateElement(const char* id);
extern KAKERA_EXPORT void kakera_DestroyElement(kakera_Element* element);
extern KAKERA_EXPORT const char* kakera_GetElementName(kakera_Element* element);
extern KAKERA_EXPORT void kakera_SetIsElementResponseEvent(kakera_Element* element, bool response);
extern KAKERA_EXPORT void kakera_BindEvedntToElement(kakera_Element* element, kakera_ElementEvents event, kakera_ElementEventCallback callback);

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_ELEMENT
