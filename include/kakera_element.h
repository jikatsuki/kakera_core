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

typedef enum {
    TEXT_STYLE_NORMAL = 0,
    TEXT_STYLE_BOLD = 1,
    TEXT_STYLE_ITALIC = 2,
    TEXT_STYLE_UNDERLINE = 4,
    TEXT_STYLE_STRIKETHROUGH = 8
} kakera_TextStyles;

typedef enum {
    PIXEL_FORMAT_RGBA8888,
    PIXEL_FORMAT_IYUV
} kakera_PixelFormats;

typedef void(*kakera_ElementEventCallback)(kakera_Element*);

extern KAKERA_EXPORT kakera_Element* kakera_CreateElement(const char* id);
extern KAKERA_EXPORT void kakera_DestroyElement(kakera_Element* element);
extern KAKERA_EXPORT void kakera_SetElementDisplaySize(kakera_Element* element, int w, int h);
extern KAKERA_EXPORT void kakera_GetElementDisplaySize(kakera_Element* element, int* w, int* h);
extern KAKERA_EXPORT void kakera_SetElementRealSize(kakera_Element* element, int w, int h);
extern KAKERA_EXPORT void kakera_GetElementRealSize(kakera_Element* element, int* w, int* h);
extern KAKERA_EXPORT void kakera_SetElementPosition(kakera_Element* element, int x, int y);
extern KAKERA_EXPORT void kakera_GetElementPosition(kakera_Element* element, int* x, int* y);
extern KAKERA_EXPORT void kakera_MoveElementViewport(kakera_Element* element, int x, int y);
extern KAKERA_EXPORT const char* kakera_GetElementName(kakera_Element* element);
extern KAKERA_EXPORT void kakera_SetIsElementResponseEvent(kakera_Element* element, bool response);
extern KAKERA_EXPORT void kakera_BindEvedntToElement(kakera_Element* element, kakera_ElementEvents event, kakera_ElementEventCallback callback);
extern KAKERA_EXPORT char* kakera_GetPixelsFromColor(int w, int h, uint8_t r, uint8_t g, uint8_t b);
extern KAKERA_EXPORT char* kakera_GetPixelsFromPicture(const char* picture);
extern KAKERA_EXPORT char* kakera_GetPixelsFromText(const char* font, int size, uint8_t r, uint8_t g, uint8_t b, int style, const char* text, int* finalW, int* finalH);
extern KAKERA_EXPORT void kakera_SetElementContentComplex(kakera_Element* element, kakera_PixelFormats format, bool isStatic, void* pixels);
extern KAKERA_EXPORT void kakera_SetElementOpacity(kakera_Element* element, uint8_t opacity);

#define kakera_SetElementContent(element,pixels) kakera_SetElementContentComplex(element, PIXEL_FORMAT_RGBA8888, true, pixels)

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_ELEMENT
