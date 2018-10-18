#ifndef KAKERA_CORE_ELEMENT
#define KAKERA_CORE_ELEMENT

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {

} kakera_ElementEvents;

typedef void(*kakera_ElementEventCallback)(kakera_Element*);

extern KAKERA_EXPORT kakera_Element* kakera_CreateElement(const char* id);
extern KAKERA_EXPORT void kakera_DestroyElement(kakera_Element* element);
extern KAKERA_EXPORT const char* kakera_GetElementName(kakera_Element* element);
extern KAKERA_EXPORT void kakera_SetIsElementResponseEvent(kakera_Element* element, bool response);
extern KAKERA_EXPORT void kakera_BindEvedntToElement(kakera_Element* element, kakera_ElementEvents event, kakera_ElementEventCallback callback);
extern KAKERA_EXPORT void kakera_SetElementContentAsColor(kakera_Element* element, uint8_t r, uint8_t g, uint8_t b);
extern KAKERA_EXPORT void kakera_SetElementContentAsPicture(kakera_Element* element, const char* picture);
extern KAKERA_EXPORT void kakera_SetElementContentAsText(kakera_Element* element, const char* font, int size, uint8_t r, uint8_t g, uint8_t b, int style, const char* text);

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_ELEMENT
