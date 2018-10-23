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
    KAKERA_ELEMENT_ON_BLUR,
    KAKERA_ELEMENT_ON_CREATE,
    KAKERA_ELEMENT_ON_CLICK,
    KAKERA_ELEMENT_ON_DESTROY,
    KAKERA_ELEMENT_ON_DOUBLE_CLICK,
    KAKERA_ELEMENT_ON_FOCUS,
    KAKERA_ELEMENT_ON_KEY_DOWN,
    KAKERA_ELEMENT_ON_KEY_UP,
    KAKERA_ELEMENT_ON_MOUSE_DOWN,
    KAKERA_ELEMENT_ON_MOUSE_ENTER,
    KAKERA_ELEMENT_ON_MOUSE_LEAVE,
    KAKERA_ELEMENT_ON_MOUSE_MOVE,
    KAKERA_ELEMENT_ON_MOUSE_UP,
    KAKERA_ELEMENT_ON_MOUSE_WHEEL_SCROLL,
    KAKERA_ELEMENT_ON_FRAME_REFRESH
} kakera_ElementEvents;

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

typedef void(*kakera_ElementEventCallback)(kakera_Element*);

extern KAKERA_EXPORT kakera_Element* kakera_CreateElement();
extern KAKERA_EXPORT void kakera_InitailizeElement(kakera_Element* element, kakera_PixelFormats format, const char* name);
extern KAKERA_EXPORT void kakera_SetElementDisplaySize(kakera_Element* element, int w, int h);
extern KAKERA_EXPORT void kakera_GetElementDisplaySize(kakera_Element* element, int* w, int* h);
extern KAKERA_EXPORT void kakera_SetElementRealSize(kakera_Element* element, int w, int h);
extern KAKERA_EXPORT void kakera_GetElementRealSize(kakera_Element* element, int* w, int* h);
extern KAKERA_EXPORT void kakera_SetElementPosition(kakera_Element* element, int x, int y);
extern KAKERA_EXPORT void kakera_GetElementPosition(kakera_Element* element, int* x, int* y);
extern KAKERA_EXPORT void kakera_MoveElementViewport(kakera_Element* element, int x, int y);
extern KAKERA_EXPORT const char* kakera_GetElementName(kakera_Element* element);
extern KAKERA_EXPORT void kakera_SetIsElementResponseEvent(kakera_Element* element, bool response);
extern KAKERA_EXPORT void kakera_BindEventToElement(kakera_Element* element, kakera_ElementEvents event, kakera_ElementEventCallback callback);
extern KAKERA_EXPORT char* kakera_GetPixelsFromColor(int w, int h, uint8_t r, uint8_t g, uint8_t b);
extern KAKERA_EXPORT char* kakera_GetPixelsFromPicture(const kakera_File * picture);
extern KAKERA_EXPORT char* kakera_GetPixelsFromText(const kakera_File * font, int size, uint8_t r, uint8_t g, uint8_t b, int style, const char * text, int* finalW, int* finalH);
extern KAKERA_EXPORT void kakera_SetElementContent(kakera_Element* element, void* pixels);
extern KAKERA_EXPORT void kakera_SetElementContentByYUVPixels(kakera_Element* element, void* YPixels, void* UPixels, void* VPixels, int YPitch, int UPitch, int VPitch);
extern KAKERA_EXPORT void kakera_SetElementOpacity(kakera_Element* element, uint8_t opacity);
extern KAKERA_EXPORT void kakera_RotateElement(kakera_Element* element, double angle);

KAKERA_DEPRECATED extern KAKERA_EXPORT void kakera_DestroyElement(kakera_Element* element);

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_ELEMENT
