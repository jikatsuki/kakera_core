#include "kakera_header.h"
#include "kakera_element.h"
#include "kakera_part_implementation.h"
#include "kakera_structs.hpp"
#include <cstring>

using namespace std;

kakera_Element * kakera_CreateElement(const char * id)
{
    kakera_Element* result = new kakera_Element;
    result->node = new Tree<kakera_Element*>::Node;
    result->node->data = result;
    result->isResponseEvent = true;
    return result;;
}

void kakera_DestroyElement(kakera_Element * element)
{
    element->scene->elementList.DeleteNode(element->node);
    delete element;
}

void kakera_SetElementDisplaySize(kakera_Element* element, int w, int h)
{
    element->displaySize.w = w;
    element->displaySize.h = h;
}

void kakera_SetElementRealSize(kakera_Element* element, int w, int h)
{
    element->realSize.w = w;
    element->realSize.h = h;
}

void kakera_SetElementPosition(kakera_Element* element, int x, int y)
{
    element->position.x = x;
    element->position.y = y;
}

void kakera_MoveElementViewport(kakera_Element* element, int x, int y)
{
    element->viewport.x = x;
    element->viewport.y = y;
}

const char * kakera_GetElementName(kakera_Element * element)
{
    return element->name;
}

void kakera_SetIsElementResponseEvent(kakera_Element * element, bool response)
{
    element->isResponseEvent = response;
}

void kakera_BindEvedntToElement(kakera_Element * element, kakera_ElementEvents event, kakera_ElementEventCallback callback)
{
    element->callbackList.emplace(event, callback);
}

void kakera_SetElementContentAsColor(kakera_Element * element, uint8_t r, uint8_t g, uint8_t b)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(0, element->realSize.w, element->realSize.h, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
    element->texture = SDL_CreateTextureFromSurface(element->scene->window->renderer, surface);
    SDL_SetTextureBlendMode(element->texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
}

void kakera_SetElementContentAsPicture(kakera_Element * element, const char * picture)
{
    SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(picture, strlen(picture)), 1);
    element->texture = SDL_CreateTextureFromSurface(element->scene->window->renderer, surface);
    SDL_SetTextureBlendMode(element->texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
}

void kakera_SetElementContentAsText(kakera_Element * element, const char * font, int size, uint8_t r, uint8_t g, uint8_t b, int style, const char * text)
{
    TTF_Font* SDLFont = TTF_OpenFontRW(SDL_RWFromConstMem(font, strlen(font)), 1, size);
    TTF_SetFontStyle(SDLFont, style);
    SDL_Surface* surface = TTF_RenderUTF8_Blended(SDLFont, text, { r, g, b });
    TTF_CloseFont(SDLFont);
    kakera_SetElementRealSize(element, surface->w, surface->h);
    element->texture = SDL_CreateTextureFromSurface(element->scene->window->renderer, surface);
    SDL_SetTextureBlendMode(element->texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
}

void kakera_SetElementContentWithRawPixels(kakera_Element* element, kakera_PixelFormats format, bool isStatic, void* pixels)
{
    int SDLFormat;
    if (format == PIXEL_FORMAT_RGBA8888)
    {
        SDLFormat = SDL_PIXELFORMAT_RGBA8888;
    }
    else
    {
        SDLFormat = SDL_PIXELFORMAT_IYUV;
    }
    int SDLAccess;
    if (isStatic)
    {
        SDLAccess = SDL_TEXTUREACCESS_STATIC;
    }
    else
    {
        SDLAccess = SDL_TEXTUREACCESS_STREAMING;
    }
    element->texture = SDL_CreateTexture(element->scene->window->renderer, SDLFormat, SDLAccess, element->realSize.w, element->realSize.h);
    SDL_SetTextureBlendMode(element->texture, SDL_BLENDMODE_BLEND);
}

void kakera_SetElementOpacity(kakera_Element * element, uint8_t opacity)
{
    SDL_SetTextureAlphaMod(element->texture, opacity);
}