#ifndef KAKERA_CORE_ELEMENT
#define KAKERA_CORE_ELEMENT

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"
#include "kakera_event.h"
#include "kakera_scene.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    KAKERA_TEXT_STYLE_NORMAL = 0,
    KAKERA_TEXT_STYLE_BOLD = 1,
    KAKERA_TEXT_STYLE_ITALIC = 2,
    KAKERA_TEXT_STYLE_UNDERLINE = 4,
    KAKERA_TEXT_STYLE_STRIKETHROUGH = 8
} kakera_TextStyles;

typedef enum {
    KAKERA_ELEMENT_TYPE_STATIC,
    KAKERA_ELEMENT_TYPE_VIDEO
} kakera_PixelFormats;

typedef enum {
    KAKERA_POSREFER_PARENT,
    KAKERA_POSREFER_WINDOW
} kakera_PositionReference;

extern KAKERA_EXPORT kakera_Element*    kakera_CreateElement();
extern KAKERA_EXPORT void               kakera_InitailizeElementComplex(kakera_Element* element, kakera_PixelFormats format, kakera_PositionReference reference, const char* name);
extern KAKERA_EXPORT void               kakera_SetElementDisplaySize(kakera_Element* element, int w, int h);
extern KAKERA_EXPORT void               kakera_GetElementDisplaySize(kakera_Element* element, int* w, int* h);
extern KAKERA_EXPORT void               kakera_SetElementRealSize(kakera_Element* element, int w, int h);
extern KAKERA_EXPORT void               kakera_GetElementRealSize(kakera_Element* element, int* w, int* h);
extern KAKERA_EXPORT void               kakera_SetElementPosition(kakera_Element* element, int x, int y);
extern KAKERA_EXPORT void               kakera_GetElementPosition(kakera_Element* element, int* x, int* y);
extern KAKERA_EXPORT void               kakera_GetElementViewport(kakera_Element* element, int* x, int* y);
extern KAKERA_EXPORT void               kakera_MoveElementViewport(kakera_Element* element, int x, int y);
extern KAKERA_EXPORT const char*        kakera_GetElementName(kakera_Element* element);
extern KAKERA_EXPORT void               kakera_SetIsElementResponseEvent(kakera_Element* element, kakera_Boolean response);
extern KAKERA_EXPORT void               kakera_BindEventToElement(kakera_Element* element, kakera_ElementEvents event, kakera_ElementEventCallback callback);
extern KAKERA_EXPORT kakera_Scene*      kakera_GetSceneFromElement(kakera_Element* element);
extern KAKERA_EXPORT kakera_Pixels*     kakera_GetPixelsFromColor(int w, int h, uint8_t r, uint8_t g, uint8_t b);
extern KAKERA_EXPORT kakera_Pixels*     kakera_GetPixelsFromPicture(kakera_File * picture, kakera_Rectangle* clipArea);
extern KAKERA_EXPORT kakera_Pixels*     kakera_GetPixelsFromText(kakera_File * font, int size, uint8_t r, uint8_t g, uint8_t b, int style, const char * text);
extern KAKERA_EXPORT void               kakera_SetElementContentComplex(kakera_Element* element, kakera_Pixels* pixels, kakera_Boolean isRepeat);
extern KAKERA_EXPORT void               kakera_SetElementContentByYUVPixels(kakera_Element* element, void* YPixels, void* UPixels, void* VPixels, int YPitch, int UPitch, int VPitch);
extern KAKERA_EXPORT void               kakera_SetElementOpacity(kakera_Element* element, uint8_t opacity);
extern KAKERA_EXPORT void               kakera_RotateElement(kakera_Element* element, double angle);
extern KAKERA_EXPORT void               kakera_StopRotateElement(kakera_Element* element);
extern KAKERA_EXPORT void               kakera_SaveDataToElement(kakera_Element* element, const char* name, void* data);
extern KAKERA_EXPORT void*              kakera_ReadDataFromElement(kakera_Element* element, const char* name);
extern KAKERA_EXPORT void               kakera_SetElementReceiveInput(kakera_Element* element, kakera_Boolean receive);
extern KAKERA_EXPORT const char*        kakera_GetElementInput(kakera_Element* element);
extern KAKERA_EXPORT const char*        kakera_GetElementEditingInput(kakera_Element* element);

extern KAKERA_EXPORT KAKERA_DEPRECATED void kakera_DestroyElement(kakera_Element** element);
extern KAKERA_EXPORT KAKERA_DEPRECATED void kakera_SetElementInput(kakera_Element* element, const char* input);

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_ELEMENT
