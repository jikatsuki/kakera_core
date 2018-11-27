#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_part_implementation.h"
#include "kakera_structs.hpp"
#include "kakera_scene.h"
#include "kakera_element.h"
#include "kakera_file.h"
#include "kakera_tools.hpp"
#include "kakera_private_apis.h"
#include <forward_list>
#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <cmath>
#include <cstring>

using namespace std;
using namespace kakera_private;

unsigned int kakera_GetWindowPosCentered()
{
    return SDL_WINDOWPOS_CENTERED;
}

kakera_Window * kakera_CreateWindow(const char * title, int x, int y, int w, int h, int flags)
{
    kakera_Window* result = new kakera_Window;
    result->window = SDL_CreateWindow(title, x, y, w, h, flags | SDL_WINDOW_ALLOW_HIGHDPI);
    result->renderer = SDL_CreateRenderer(result->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(result->renderer, 255, 255, 255, 255);
    result->FPSSem = SDL_CreateSemaphore(1);
    result->isQuit = false;
    return result;
}

int kakera_DestroyWindow(kakera_Window ** window)
{
    kakera_private::CheckNullPointer(*window);
    SDL_DestroySemaphore((*window)->FPSSem);
    SDL_DestroyRenderer((*window)->renderer);
    SDL_DestroyWindow((*window)->window);
    delete *window;
    (*window) = nullptr;
    return 0;
}

void kakera_GetWindowSize(kakera_Window * window, int * w, int * h)
{
    kakera_private::CheckNullPointer(window);
    SDL_GetWindowSize(window->window, w, h);
}

void kakera_SetWindowSize(kakera_Window * window, int w, int h)
{
    kakera_private::CheckNullPointer(window);
    SDL_SetWindowSize(window->window, w, h);
}

void kakera_GetWindowPosition(kakera_Window * window, int * x, int * y)
{
    kakera_private::CheckNullPointer(window);
    SDL_GetWindowPosition(window->window, x, y);
}

void kakera_SetWindowPosition(kakera_Window * window, int x, int y)
{
    kakera_private::CheckNullPointer(window);
    SDL_SetWindowPosition(window->window, x, y);
}

const char* kakera_GetWindowTitle(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    return SDL_GetWindowTitle(window->window);
}

void kakera_SetWindowTitle(kakera_Window * window, const char * title)
{
    kakera_private::CheckNullPointer(window);
    SDL_SetWindowTitle(window->window, title);
}

void * kakera_GetWindowUserdata(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    return window->userdata;
}

void kakera_ShowWindow(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    SDL_ShowWindow(window->window);
}

void kakera_HideWindow(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    SDL_HideWindow(window->window);
}

void kakera_MinimizeWindow(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    SDL_MinimizeWindow(window->window);
}

void kakera_MaximizeWindow(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    SDL_MaximizeWindow(window->window);
}

void kakera_RaiseWindow(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    SDL_RaiseWindow(window->window);
}

void kakera_SetWindowResizeable(kakera_Window * window, kakera_Boolean resizeable)
{
    kakera_private::CheckNullPointer(window);
    SDL_bool flag;
    if (kakera_private::ConvertBool(resizeable))
    {
        flag = SDL_TRUE;
    }
    else
    {
        flag = SDL_FALSE;
    }
    SDL_SetWindowResizable(window->window, flag);
}

void kakera_SetWindowIcon(kakera_Window * window, kakera_File* icon)
{
    kakera_private::CheckNullPointer(window);
    SDL_Surface* iconSurface = IMG_Load_RW(SDL_RWFromConstMem(icon->data, icon->size), 1);
    delete icon;
    SDL_SetWindowIcon(window->window, iconSurface);
    SDL_FreeSurface(iconSurface);
}

kakera_WindowFPS kakera_GetWindowFPS(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    return window->FPS;
}

void kakera_SetWindowFPS(kakera_Window * window, kakera_WindowFPS FPS)
{
    kakera_private::CheckNullPointer(window);
    window->FPS = FPS;
}

kakera_Event * kakera_GetWindowEvent(kakera_Window * window)
{
    kakera_private::CheckNullPointer(window);
    return &window->event;
}

void kakera_SetUsingDirtyRectRender(kakera_Window * window, kakera_Boolean isUse)
{
    kakera_private::CheckNullPointer(window);
    window->usingDirtyRect = kakera_private::ConvertBool(isUse);
}

void kakera_private_ClearRenderRect(kakera_Window * window, SDL_Rect* rect)
{
    if (rect == nullptr)
    {
        SDL_RenderClear(window->renderer);
    }
    else
    {
        SDL_RenderFillRect(window->renderer, rect);
    }
}

void kakera_private_RefreshRect(kakera_Window * window, SDL_Rect* refreshArea)
{
    kakera_private_ClearRenderRect(window, refreshArea);
    if (window->activeScene != nullptr)
    {
        for (auto element : *window->activeScene->elementList)
        {
            SDL_Rect elementPositionAndSize = {
                element->position.x,
                element->position.y,
                element->displaySize.w,
                element->displaySize.h
            };
            if (kakera_private::Is2RectIntersected(refreshArea, &elementPositionAndSize))
            {
                SDL_Rect* dirtyArea = kakera_private::Get2RectIntersection(refreshArea, &elementPositionAndSize);
                SDL_Rect cropArea;
                if (dirtyArea->w < element->displaySize.w || dirtyArea->h < element->displaySize.h)
                {
                    cropArea = {
                        static_cast<int>((static_cast<float>(element->realSize.w) / static_cast<float>(element->displaySize.w)) * (element->displaySize.w - dirtyArea->w)),
                        static_cast<int>((static_cast<float>(element->realSize.h) / static_cast<float>(element->displaySize.h)) * (element->displaySize.h - dirtyArea->h)),
                        static_cast<int>((static_cast<float>(dirtyArea->w) / static_cast<float>(element->displaySize.w)) * element->realSize.w),
                        static_cast<int>((static_cast<float>(dirtyArea->h) / static_cast<float>(element->displaySize.h)) * element->realSize.h)
                    };
                }
                else
                {
                    cropArea = {
                        0,
                        0,
                        element->realSize.w,
                        element->realSize.h
                    };
                }
                
                if (element->renderInfo.isRender && element->texture != nullptr)
                {
                    SDL_RenderCopyEx(
                        window->renderer,
                        element->texture,
                        &cropArea,
                        dirtyArea,
                        element->rotateAngle,
                        NULL,
                        SDL_FLIP_NONE
                    );
                }
                delete dirtyArea;
            }
        }
    }
    SDL_RenderPresent(window->renderer);
}

int kakera_private_EventFilter(void * userdata, SDL_Event * event)
{
    kakera_Window* window = static_cast<kakera_Window*>(userdata);

    if (event->window.windowID == SDL_GetWindowID(window->window))
    {
        switch (event->type)
        {
        case SDL_QUIT:            
            window->isQuit = true;
            break;
        case SDL_WINDOWEVENT:
        {
            switch (event->window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                window->isQuit = true;
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                kakera_private::RefreshAll(window);
                break;
            default:
                break;
            }            
        }
        break;
        case SDL_MOUSEMOTION:
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            window->event.mouse.pointer.x = mouseX;
            window->event.mouse.pointer.y = mouseY;
            for (auto iter = window->activeScene->elementList->rbegin(); iter != window->activeScene->elementList->rend(); iter++)
            {
                auto element = *iter;
                if (element->isResponseEvent)
                {
                    if (isPointInArea(mouseX, mouseY, element->renderInfo.positionAndSize))
                    {
                        if (!element->isMouseEntered)
                        {
                            if (window->activeScene->mouseEnteredElement != nullptr)
                            {
                                window->activeScene->mouseEnteredElement->isMouseEntered = false;
                                kakera_private::RunCallback(window->activeScene->mouseEnteredElement, KAKERA_ELEMENT_ON_MOUSE_LEAVE);
                            }
                            element->isMouseEntered = true;
                            kakera_private::RunCallback(element, KAKERA_ELEMENT_ON_MOUSE_ENTER);
                            window->activeScene->mouseEnteredElement = element;
                        }
                        kakera_private::RunCallback(element, KAKERA_ELEMENT_ON_MOUSE_MOVE);
                        break;
                    }
                }
            }
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            window->event.mouse.pointer.x = mouseX;
            window->event.mouse.pointer.y = mouseY;
            switch (event->button.button)
            {
            case SDL_BUTTON_LEFT:
                window->event.mouse.button = KAKERA_MOUSE_LEFT_BUTTON;
                break;
            case SDL_BUTTON_MIDDLE:
                window->event.mouse.button = KAKERA_MOUSE_MIDDLE_BUTTON;
                break;
            case SDL_BUTTON_RIGHT:
                window->event.mouse.button = KAKERA_MOUSE_RIGHT_BUTTON;
                break;
            default:
                break;
            }
            for (auto iter = window->activeScene->elementList->rbegin(); iter != window->activeScene->elementList->rend(); iter++)
            {
                auto element = *iter;
                if (element->isResponseEvent)
                {
                    if (isPointInArea(mouseX, mouseY, element->renderInfo.positionAndSize) &&
                        (event->button.button == SDL_BUTTON_LEFT ||
                            event->button.button == SDL_BUTTON_RIGHT ||
                            event->button.button == SDL_BUTTON_MIDDLE
                            ) &&
                        event->button.state == SDL_PRESSED)
                    {
                        kakera_private::RunCallback(element, KAKERA_ELEMENT_ON_MOUSE_DOWN);
                        break;
                    }
                }
            }
        }
        break;
        case SDL_MOUSEBUTTONUP:
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            window->event.mouse.pointer.x = mouseX;
            window->event.mouse.pointer.y = mouseY;
            switch (event->button.button)
            {
            case SDL_BUTTON_LEFT:
                window->event.mouse.button = KAKERA_MOUSE_LEFT_BUTTON;
                break;
            case SDL_BUTTON_MIDDLE:
                window->event.mouse.button = KAKERA_MOUSE_MIDDLE_BUTTON;
                break;
            case SDL_BUTTON_RIGHT:
                window->event.mouse.button = KAKERA_MOUSE_RIGHT_BUTTON;
                break;
            default:
                break;
            }
            for (auto iter = window->activeScene->elementList->rbegin(); iter != window->activeScene->elementList->rend(); iter++)
            {
                auto element = *iter;
                if (element->isResponseEvent)
                {
                    if (isPointInArea(mouseX, mouseY, element->renderInfo.positionAndSize) &&
                        (event->button.button == SDL_BUTTON_LEFT ||
                            event->button.button == SDL_BUTTON_RIGHT ||
                            event->button.button == SDL_BUTTON_MIDDLE
                            ) &&
                        event->button.state == SDL_RELEASED)
                    {
                        if (event->button.clicks == 2)
                        {
                            kakera_private::RunCallback(element, KAKERA_ELEMENT_ON_DOUBLE_CLICK);
                        }
                        else if (event->button.clicks == 1)
                        {
                            kakera_private::RunCallback(element, KAKERA_ELEMENT_ON_CLICK);
                        }
                        kakera_private::RunCallback(element, KAKERA_ELEMENT_ON_MOUSE_UP);
                        kakera_SetFocusElement(window->activeScene, element);
                        break;
                    }
                }
            }
        }
        break;
        case SDL_MOUSEWHEEL:
        {
            if (window->activeScene->focusElement != nullptr &&
                window->activeScene->focusElement->isResponseEvent)
            {
                if (event->wheel.x != 0)
                {
                    window->event.wheel.direction = event->wheel.x > 0 ? KAKERA_MOUSE_WHEEL_RIGHT : KAKERA_MOUSE_WHEEL_LEFT;
                    window->event.wheel.distance = abs(event->wheel.x);
                }
                if (event->wheel.y != 0)
                {
                    window->event.wheel.direction = event->wheel.y > 0 ? KAKERA_MOUSE_WHEEL_DOWN : KAKERA_MOUSE_WHEEL_UP;
                    window->event.wheel.distance = abs(event->wheel.y);
                }
                kakera_private::RunCallback(window->activeScene->focusElement, KAKERA_ELEMENT_ON_MOUSE_WHEEL_SCROLL);
            }            
        }
        break;
        case SDL_KEYDOWN:
        {
            if (window->activeScene->focusElement != nullptr &&
                window->activeScene->focusElement->isResponseEvent)
            {
                window->event.keyboard.key = static_cast<kakera_KeyboardKey>(event->key.keysym.scancode);
                kakera_private::RunCallback(window->activeScene->focusElement, KAKERA_ELEMENT_ON_KEY_DOWN);
            }
            break;
        }
        case SDL_KEYUP:
        {
            if (window->activeScene->focusElement != nullptr &&
                window->activeScene->focusElement->isResponseEvent)
            {
                window->event.keyboard.key = static_cast<kakera_KeyboardKey>(event->key.keysym.scancode);
                kakera_private::RunCallback(window->activeScene->focusElement, KAKERA_ELEMENT_ON_KEY_UP);
            }            
        }
        break;
        case SDL_TEXTEDITING:
        {
            if (window->activeScene->focusElement != nullptr &&
                window->activeScene->focusElement->isResponseEvent &&
                window->activeScene->focusElement->isReceiveInput)
            {
                window->activeScene->focusElement->receivedEditingInput = string(event->edit.text);
                auto inputRect = new SDL_Rect({
                        window->activeScene->focusElement->renderInfo.positionAndSize->x + window->activeScene->focusElement->renderInfo.positionAndSize->w + event->edit.length,
                        window->activeScene->focusElement->renderInfo.positionAndSize->y + window->activeScene->focusElement->renderInfo.positionAndSize->h / 2,
                        0,
                        0
                    });
                SDL_SetTextInputRect(inputRect);
                delete inputRect;
                kakera_private::RunCallback(window->activeScene->focusElement, KAKERA_ELEMENT_ON_TEXT_INPUT);
            }            
        }
        break;
        case SDL_TEXTINPUT:
        {
            if (window->activeScene->focusElement != nullptr &&
                window->activeScene->focusElement->isResponseEvent &&
                window->activeScene->focusElement->isReceiveInput)
            {
                window->activeScene->focusElement->receivedInput += string(event->text.text);
                kakera_private::RunCallback(window->activeScene->focusElement, KAKERA_ELEMENT_ON_TEXT_INPUT);
            }            
        }
        break;
        default:
            break;
        }
    }
    
    return 0;
}

void kakera_StartWindow(kakera_Window ** window, kakera_WindowRenderMode mode, void* userdata)
{
    kakera_private::CheckNullPointer(*window);
    (*window)->userdata = userdata;
    SDL_Event event;
    SDL_AddEventWatch(kakera_private_EventFilter, *window);
    SDL_StartTextInput();
    kakera_private::RefreshAll(*window);
    if (mode == KAKERA_RENDER_GAME)
    {
        (*window)->isGameRender = true;
        while (!(*window)->isQuit)
        {
            SDL_TimerID FPSTimer = SDL_AddTimer(1000 / (*window)->FPS, [](Uint32 interval, void * param) -> Uint32 {
                kakera_Window* window = reinterpret_cast<kakera_Window*>(param);
                SDL_SemPost(window->FPSSem);
                return 0;
            }, *window);            
            SDL_PollEvent(&event);
            if ((*window)->usingDirtyRect)
            {
                if ((*window)->activeScene->dirtyArea != nullptr)
                {
                    kakera_private::RefreshRect(*window, (*window)->activeScene->dirtyArea);
                    delete (*window)->activeScene->dirtyArea;
                    (*window)->activeScene->dirtyArea = nullptr;
                }
            }
            else
            {
                kakera_private::RefreshAll(*window);
            }
            SDL_SemWait((*window)->FPSSem);
            SDL_RemoveTimer(FPSTimer);
        }
    }
    else
    {
        while (!(*window)->isQuit)
        {
            SDL_TimerID FPSTimer = SDL_AddTimer(1000 / (*window)->FPS, [](Uint32 interval, void * param) -> Uint32 {
                kakera_Window* window = reinterpret_cast<kakera_Window*>(param);
                SDL_SemPost(window->FPSSem);
                return 0;
            }, *window);
            SDL_PollEvent(&event);
            if ((*window)->activeScene->isRefresh)
            {
                if ((*window)->usingDirtyRect)
                {
                    if ((*window)->activeScene->dirtyArea != nullptr)
                    {
                        kakera_private::RefreshRect(*window, (*window)->activeScene->dirtyArea);
                        delete (*window)->activeScene->dirtyArea;
                        (*window)->activeScene->dirtyArea = nullptr;
                    }
                }
                else
                {
                    kakera_private::RefreshAll(*window);
                }
                (*window)->activeScene->isRefresh = false;
            }
            SDL_SemWait((*window)->FPSSem);
            SDL_RemoveTimer(FPSTimer);
        }
    }    
    if ((*window)->isQuit)
    {
        SDL_DelEventWatch(kakera_private_EventFilter, *window);
        SDL_StopTextInput();
        kakera_DestroyWindow(window);
        return;
    }
}
