#include "kakera_header.h"
#include "kakera_element.h"
#include "kakera_part_implementation.h"
#include "kakera_structs.hpp"
#include "kakera_tools.hpp"
#include "kakera_private_apis.h"
#include <cstring>
#include <mutex>

using namespace std;
using namespace kakera_private;

kakera_Element * kakera_CreateElement()
{
    auto result = new kakera_Element;
    result->node = new Tree<kakera_Element*>::Node;
    result->node->data = result;
    result->renderInfo.positionAndSize = new SDL_Rect;
    result->renderInfo.cropArea = new SDL_Rect;
    return result;
}

void kakera_InitailizeElementComplex(kakera_Element * element, kakera_PixelFormats format, kakera_PositionReference reference, const char * name)
{
    kakera_private::CheckNullPointer(element);
    element->name = name;
    element->reference = reference;
    if (format == KAKERA_ELEMENT_TYPE_STATIC)
    {
        element->SDLFormat = SDL_PIXELFORMAT_RGBA8888;
        element->SDLAccess = SDL_TEXTUREACCESS_STATIC;
    }
    else
    {
        element->SDLFormat = SDL_PIXELFORMAT_IYUV;
        element->SDLAccess = SDL_TEXTUREACCESS_STREAMING;
    }
    element->renderInfo.isRender = true;
    kakera_private::RunCallback(element, KAKERA_ELEMENT_ON_CREATE);
}

void kakera_DestroyElement(kakera_Element ** element)
{
    kakera_private::CheckNullPointer(*element);
    kakera_private::RunCallback((*element), KAKERA_ELEMENT_ON_DESTROY);
    (*element)->callbackList.clear();
    (*element)->dataList.clear();
    delete (*element)->renderInfo.positionAndSize;
    delete (*element)->renderInfo.cropArea;
    kakera_DeleteElementFromScene((*element)->scene, (*element));
    *element = nullptr;
}

void kakera_SetElementDisplaySize(kakera_Element* element, int w, int h)
{
    kakera_private::CheckNullPointer(element);    
    SDL_Rect oldrect, newrect;
    oldrect = {
        element->position.x,
        element->position.y,
        element->displaySize.w,
        element->displaySize.h
    };
    newrect = {
        element->position.x,
        element->position.y,
        w,
        h
    };
    element->displaySize.w = w;
    element->displaySize.h = h;
    if (element->scene->window->usingDirtyRect)
    {
        RefreshInfo info;
        info.moved.newPosition = newrect;
        info.moved.oldPosition = oldrect;
        kakera_private::PushRefreshEvent(element->scene, RefreshType::Refresh_Moved, info);
    }
    else
    {
        kakera_private::PushRefreshEvent(element->scene);
    }
    //kakera_private::PushRefreshEvent(element->scene);
}

void kakera_GetElementDisplaySize(kakera_Element * element, int * w, int * h)
{
    kakera_private::CheckNullPointer(element);    
    *w = element->displaySize.w;
    *h = element->displaySize.h;
}

void kakera_SetElementRealSize(kakera_Element* element, int w, int h)
{
    kakera_private::CheckNullPointer(element);    
    element->resizeFlag = true;
    element->realSize.w = w;
    element->realSize.h = h;
}

void kakera_GetElementRealSize(kakera_Element * element, int * w, int * h)
{
    kakera_private::CheckNullPointer(element);   
    *w = element->realSize.w;
    *h = element->realSize.h;
}

void kakera_SetElementPosition(kakera_Element* element, int x, int y)
{
    kakera_private::CheckNullPointer(element);    
    SDL_Rect oldrect, newrect;
    oldrect = {
        element->position.x,
        element->position.y,
        element->displaySize.w,
        element->displaySize.h
    };
    newrect = {
        x,
        y,
        element->displaySize.w,
        element->displaySize.h
    };
    element->position.x = x;
    element->position.y = y;
    if (element->scene->window->usingDirtyRect)
    {
        RefreshInfo info;
        info.moved.newPosition = newrect;
        info.moved.oldPosition = oldrect;
        kakera_private::PushRefreshEvent(element->scene, RefreshType::Refresh_Moved, info);
    }
    else
    {
        kakera_private::PushRefreshEvent(element->scene);
    }
    //kakera_private::PushRefreshEvent(element->scene);
}

void kakera_GetElementPosition(kakera_Element * element, int * x, int * y)
{
    kakera_private::CheckNullPointer(element);    
    *x = element->position.x;
    *y = element->position.y;
}

void kakera_GetElementViewport(kakera_Element * element, int * x, int * y)
{
    kakera_private::CheckNullPointer(element);    
    *x = element->viewport.x;
    *y = element->viewport.y;
}

void kakera_MoveElementViewport(kakera_Element* element, int x, int y)
{
    kakera_private::CheckNullPointer(element);    
    element->viewport.x = x;
    element->viewport.y = y;
    if (element->scene->window->usingDirtyRect)
    {
        RefreshInfo info;
        info.unmoved.refreshArea = {
            element->position.x,
            element->position.y,
            element->displaySize.w,
            element->displaySize.h
        };
        kakera_private::PushRefreshEvent(element->scene, RefreshType::Refresh_Unmoved, info);
    }
    else
    {
        kakera_private::PushRefreshEvent(element->scene);
    }
    //kakera_private::PushRefreshEvent(element->scene);
}

const char * kakera_GetElementName(kakera_Element * element)
{
    kakera_private::CheckNullPointer(element);    
    return element->name;
}

void kakera_SetIsElementResponseEvent(kakera_Element * element, kakera_Boolean response)
{
    kakera_private::CheckNullPointer(element);    
    element->isResponseEvent = kakera_private::ConvertBool(response);
}

void kakera_BindEventToElement(kakera_Element * element, kakera_ElementEvents event, kakera_ElementEventCallback callback)
{
    kakera_private::CheckNullPointer(element);    
    element->callbackList.emplace(event, callback);
}

kakera_Scene * kakera_GetSceneFromElement(kakera_Element * element)
{
    kakera_private::CheckNullPointer(element);    
    return element->scene;
}

kakera_Pixels * kakera_GetPixelsFromColor(int w, int h, uint8_t r, uint8_t g, uint8_t b)
{
    SDL_Surface* RAWSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(RAWSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(RAWSurface);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));        
    kakera_Pixels* result = new kakera_Pixels;
    result->w = w;
    result->h = h;
    int pixelSize = surface->pitch * surface->h;
    result->pixels = new char[pixelSize];
    memmove(result->pixels, surface->pixels, pixelSize);
    SDL_FreeSurface(surface);
    return result;
}

kakera_Pixels * kakera_GetPixelsFromPicture(kakera_File * picture, kakera_Rectangle* clipArea)
{
    kakera_private::CheckNullPointer(picture);
    SDL_Surface* RAWSurface = IMG_Load_RW(SDL_RWFromConstMem(picture->data, picture->size), 1);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(RAWSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(RAWSurface);
    kakera_Pixels* result = new kakera_Pixels;
    if (clipArea == nullptr)
    {
        result->w = surface->w;
        result->h = surface->h;
        int pixelSize = surface->pitch * surface->h;
        result->pixels = new char[pixelSize];
        memmove(result->pixels, surface->pixels, pixelSize);
        SDL_FreeSurface(surface);
    }
    else
    {
        SDL_Surface* clipSurface = SDL_CreateRGBSurfaceWithFormat(0, clipArea->w, clipArea->h, 32, SDL_PIXELFORMAT_RGBA8888);
        SDL_Rect* clipRect = kakera_private::ConvertRect(clipArea);
        SDL_BlitSurface(surface, clipRect, clipSurface, NULL);
        delete clipRect;
        SDL_FreeSurface(surface);
        result->w = clipArea->w;
        result->h = clipArea->h;
        int pixelSize = clipSurface->pitch * clipSurface->h;
        result->pixels = new char[pixelSize];
        memmove(result->pixels, clipSurface->pixels, pixelSize);
        SDL_FreeSurface(clipSurface);
    }
    return result;
}

kakera_Pixels * kakera_GetPixelsFromText(kakera_File * font, int size, uint8_t r, uint8_t g, uint8_t b, int style, const char * text)
{
    kakera_private::CheckNullPointer(font);
    TTF_Font* SDLFont = TTF_OpenFontRW(SDL_RWFromConstMem(font->data, font->size), 1, size);
    TTF_SetFontStyle(SDLFont, style);
    SDL_Surface* RAWSurface = TTF_RenderUTF8_Blended(SDLFont, text, { r, g, b });
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(RAWSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(RAWSurface);
    TTF_CloseFont(SDLFont);    
    kakera_Pixels* result = new kakera_Pixels;
    result->w = surface->w;
    result->h = surface->h;
    int pixelSize = surface->pitch * surface->h;
    result->pixels = new char[pixelSize];
    memmove(result->pixels, surface->pixels, pixelSize);
    SDL_FreeSurface(surface);
    return result;
}

void kakera_SetElementContentComplex(kakera_Element* element, kakera_Pixels* pixels, kakera_Boolean isRepeat)
{
    kakera_private::CheckNullPointer(element);    
    if (element->resizeFlag && element->texture != nullptr)
    {
        element->resizeFlag = false;
        SDL_DestroyTexture(element->texture);
        element->texture = nullptr;
    }
    if (element->texture == nullptr)
    {
        element->texture = SDL_CreateTexture(element->scene->window->renderer, element->SDLFormat, element->SDLAccess, element->realSize.w, element->realSize.h);
        SDL_SetTextureBlendMode(element->texture, SDL_BLENDMODE_BLEND);
    }

    if (kakera_private::ConvertBool(isRepeat))
    {
        int extraW, extraH;
        int repeatTimeX = element->realSize.w / pixels->w;
        int repeatTimeY = element->realSize.h / pixels->h;
        bool specialX = false, specialY = false;
        if (element->realSize.w % pixels->w == 0)
            extraW = 0;
        else
        {
            extraW = element->realSize.w - pixels->w * repeatTimeX;
            specialX = true;
        }

        if (element->realSize.h % pixels->h == 0)
            extraH = 0;
        else
        {
            extraH = element->realSize.h - pixels->h * repeatTimeY;
            specialY = true;
        }

        SDL_Rect fillRect = { 0, 0, pixels->w, pixels->h };
        for (int i = 0; i < repeatTimeY; i++)
        {
            fillRect.y = fillRect.y + i * pixels->h;
            for (int j = 0; j < repeatTimeX; j++)
            {
                fillRect.x = fillRect.x + j * pixels->w;
                SDL_UpdateTexture(element->texture, &fillRect, pixels->pixels, pixels->w * 4);
            }
            fillRect.x = 0;
        }

        if (specialX)
        {
            fillRect = { element->realSize.w - extraW, 0, extraW, pixels->h };
            SDL_Rect clipArea = { 0, 0, extraW, pixels->h };
            kakera_Pixels* cliped = kakera_private::ClipPixels(pixels, &clipArea);
            for (int i = 0; i < repeatTimeY; i++)
            {
                fillRect.y = fillRect.y + i * pixels->h;
                SDL_UpdateTexture(element->texture, &fillRect, cliped->pixels, extraW * 4);
            }
            kakera_private::DeletePixels(&cliped);
        }

        if (specialY)
        {
            fillRect = { 0, element->realSize.h - extraH, pixels->w, extraH };
            SDL_Rect clipArea = { 0, 0, pixels->w, extraH };
            kakera_Pixels* cliped = kakera_private::ClipPixels(pixels, &clipArea);
            for (int i = 0; i < repeatTimeX; i++)
            {
                fillRect.x = fillRect.x + i * pixels->w;
                SDL_UpdateTexture(element->texture, &fillRect, pixels->pixels, pixels->w * 4);
            }
            kakera_private::DeletePixels(&cliped);
        }

        if (specialX && specialY)
        {
            fillRect = { element->realSize.w - extraW, element->realSize.h - extraH, extraW, extraH };
            SDL_Rect clipArea = { 0, 0, extraW, extraH };
            kakera_Pixels* cliped = kakera_private::ClipPixels(pixels, &clipArea);
            SDL_UpdateTexture(element->texture, &fillRect, cliped->pixels, extraW * 4);
            kakera_private::DeletePixels(&cliped);
        }
    }
    else
    {
        SDL_UpdateTexture(element->texture, NULL, pixels->pixels, element->realSize.w * 4);
    }

    kakera_private::DeletePixels(&pixels);

    if (element->scene->window->usingDirtyRect)
    {
        RefreshInfo info;
        info.unmoved.refreshArea = {
            element->position.x,
            element->position.y,
            element->displaySize.w,
            element->displaySize.h
        };
        kakera_private::PushRefreshEvent(element->scene, RefreshType::Refresh_Unmoved, info);
    }
    else
    {
        kakera_private::PushRefreshEvent(element->scene);
    }
    //kakera_private::PushRefreshEvent(element->scene);
}

void kakera_SetElementContentByYUVPixels(kakera_Element * element, void * YPixels, void * UPixels, void * VPixels, int YPitch, int UPitch, int VPitch)
{
    kakera_private::CheckNullPointer(element);    
    if (element->SDLFormat != SDL_PIXELFORMAT_IYUV)
    {
        return;
    }
    if (element->texture == nullptr)
    {
        element->texture = SDL_CreateTexture(element->scene->window->renderer, element->SDLFormat, element->SDLAccess, element->realSize.w, element->realSize.h);
        SDL_SetTextureBlendMode(element->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_UpdateYUVTexture(element->texture, NULL, static_cast<Uint8*>(YPixels), YPitch, static_cast<Uint8*>(UPixels), UPitch, static_cast<Uint8*>(VPixels), VPitch);
    if (element->scene->window->usingDirtyRect)
    {
        RefreshInfo info;
        info.unmoved.refreshArea = {
            element->position.x,
            element->position.y,
            element->displaySize.w,
            element->displaySize.h
        };
        kakera_private::PushRefreshEvent(element->scene, RefreshType::Refresh_Unmoved, info);
    }
    else
    {
        kakera_private::PushRefreshEvent(element->scene);
    }
    //kakera_private::PushRefreshEvent(element->scene);
}

void kakera_SetElementOpacity(kakera_Element * element, uint8_t opacity)
{
    kakera_private::CheckNullPointer(element);    
    SDL_SetTextureAlphaMod(element->texture, opacity);
    if (!element->node->children.empty())
    {
        for (auto node : element->node->children)
        {
            SDL_SetTextureAlphaMod(node->data->texture, opacity);
        }
    }
    if (element->scene->window->usingDirtyRect)
    {
        RefreshInfo info;
        info.unmoved.refreshArea = {
            element->position.x,
            element->position.y,
            element->displaySize.w,
            element->displaySize.h
        };
        kakera_private::PushRefreshEvent(element->scene, RefreshType::Refresh_Unmoved, info);
    }
    else
    {
        kakera_private::PushRefreshEvent(element->scene);
    }
    //kakera_private::PushRefreshEvent(element->scene);
}

void kakera_RotateElement(kakera_Element * element, double angle)
{
    kakera_private::CheckNullPointer(element);
    if (!element->isRotating)
    {
        element->isRotating = true;
    }
    element->rotateAngle = angle;
    if (!element->node->children.empty())
    {
        for (auto node : element->node->children)
        {
            node->data->rotateAngle = angle;
        }
    }
    if (element->scene->window->usingDirtyRect)
    {
        RefreshInfo info;
        info.rotate.positionAndSize = {
            element->position.x,
            element->position.y,
            element->displaySize.w,
            element->displaySize.h
        };
        kakera_private::PushRefreshEvent(element->scene, RefreshType::Refresh_Rotate, info);
    }
    else
    {
        kakera_private::PushRefreshEvent(element->scene);
    }
    //kakera_private::PushRefreshEvent(element->scene);
}

void kakera_StopRotateElement(kakera_Element * element)
{
    kakera_private::CheckNullPointer(element);
    element->isRotating = false;
    element->rotateAngle = 0.0;
}

void kakera_SaveDataToElement(kakera_Element * element, const char * name, void * data)
{
    kakera_private::CheckNullPointer(element);
    element->dataList.emplace(name, data);
}

void * kakera_ReadDataFromElement(kakera_Element * element, const char * name)
{
    kakera_private::CheckNullPointer(element);    
    auto iter = element->dataList.find(name);
    if (iter == element->dataList.end())
    {
        return nullptr;
    }
    else
    {
        return iter->second;
    }
}

void kakera_SetElementReceiveInput(kakera_Element * element, kakera_Boolean receive)
{
    kakera_private::CheckNullPointer(element);    
    element->isReceiveInput = receive;
}

const char * kakera_GetElementInput(kakera_Element * element)
{
    kakera_private::CheckNullPointer(element);    
    if (element->isReceiveInput)
    {
        return element->receivedInput.c_str();
    }
    return nullptr;
}

const char * kakera_GetElementEditingInput(kakera_Element * element)
{
    kakera_private::CheckNullPointer(element);    
    if (element->isReceiveInput)
    {
        return element->receivedEditingInput.c_str();
    }
    return nullptr;
}

void kakera_SetElementInput(kakera_Element * element, const char * input)
{
    kakera_private::CheckNullPointer(element);    
    element->receivedInput = input;
}
