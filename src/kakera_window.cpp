#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_part_implementation.h"
#include "kakera_structs.hpp"
#include "kakera_element.h"
#include "kakera_tools.hpp"
#include <forward_list>
#include <iostream>

using namespace std;

unsigned int kakera_GetWindowPosCentered()
{
    return SDL_WINDOWPOS_CENTERED;
}

kakera_Window * kakera_CreateWindow(const char * title, int x, int y, int w, int h, int flags)
{
    kakera_Window* result = new kakera_Window;
    result->window = SDL_CreateWindow(title, x, y, w, h, flags);
    result->renderer = SDL_CreateRenderer(result->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(result->renderer, 255, 255, 255, 255);
    result->FPSSem = SDL_CreateSemaphore(1);
    result->isQuit = false;
    return result;
}

int kakera_DestroyWindow(kakera_Window * window)
{
    SDL_DestroySemaphore(window->FPSSem);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    delete window;
    return 0;
}

void kakera_GetWindowSize(kakera_Window * window, int * w, int * h)
{
    SDL_GetWindowSize(window->window, w, h);
}

void kakera_SetWindowSize(kakera_Window * window, int w, int h)
{
    SDL_SetWindowSize(window->window, w, h);
}

void kakera_GetWindowPosition(kakera_Window * window, int * x, int * y)
{
    SDL_GetWindowPosition(window->window, x, y);
}

void kakera_SetWindowPosition(kakera_Window * window, int x, int y)
{
    SDL_SetWindowPosition(window->window, x, y);
}

const char* kakera_GetWindowTitle(kakera_Window * window)
{
    return SDL_GetWindowTitle(window->window);
}

void kakera_SetWindowTitle(kakera_Window * window, const char * title)
{
    SDL_SetWindowTitle(window->window, title);
}

void kakera_ShowWindow(kakera_Window * window)
{
    SDL_ShowWindow(window->window);
}

void kakera_HideWindow(kakera_Window * window)
{
    SDL_HideWindow(window->window);
}

void kakera_MinimizeWindow(kakera_Window * window)
{
    SDL_MinimizeWindow(window->window);
}

void kakera_MaximizeWindow(kakera_Window * window)
{
    SDL_MaximizeWindow(window->window);
}

void kakera_RaiseWindow(kakera_Window * window)
{
    SDL_RaiseWindow(window->window);
}

void kakera_SetWindowResizeable(kakera_Window * window, bool resizeable)
{
    SDL_bool flag;
    if (resizeable)
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
    SDL_Surface* iconSurface = IMG_Load_RW(SDL_RWFromConstMem(icon->data, icon->size), 1);
    delete icon;
    SDL_SetWindowIcon(window->window, iconSurface);
    SDL_FreeSurface(iconSurface);
}

void kakera_SetWindowFPS(kakera_Window * window, kakera_WindowFPS FPS)
{
    window->FPS = FPS;
}

int kakera_private_EventFilter(void * userdata, SDL_Event * event)
{
    kakera_Window* window = static_cast<kakera_Window*>(userdata);
    switch (event->type)
    {
    case SDL_QUIT:
        window->isQuit = true;
        break;
    default:
        break;
    }
    return 0;
}

Uint32 kakera_private_FPSSemCallback(Uint32 interval, void * param)
{
    kakera_Window* window = static_cast<kakera_Window*>(param);
    SDL_SemPost(window->FPSSem);
    return 0;
}

void kakera_pirvate_RefreshFrame(kakera_Window * window)
{
    SDL_RenderClear(window->renderer);
    if (window->activeScene != nullptr)
    {
        forward_list<kakera_Element*> elementList;
        window->activeScene->elementList.BreadthFirstSearch([&elementList](Tree<kakera_Element*>::Node* node) {
            elementList.emplace_front(node->data);
        });
        elementList.reverse();
        for (auto element : elementList)
        {
            kakera_RunCallback(element, KAKERA_ELEMENT_ON_FRAME_REFRESH);
            switch (element->reference)
            {
            case KAKERA_POSREFER_PARENT:
            {
                if (element->node == window->activeScene->elementList.GetRoot())
                {
                    int window_w, window_h;
                    kakera_GetWindowSize(window, &window_w, &window_h);
                    kakera_SetElementPosition(element, 0, 0);
                    kakera_SetElementDisplaySize(element, window_w, window_h);
                    element->renderInfo.positionAndSize->x = 0;
                    element->renderInfo.positionAndSize->y = 0;
                    element->renderInfo.positionAndSize->w = window_w;
                    element->renderInfo.positionAndSize->h = window_h;
                }
                else
                {
                    int viewportX1 = element->node->parent->data->viewport.x;
                    int viewportX2 = element->node->parent->data->viewport.x + element->node->parent->data->displaySize.w;
                    int viewportY1 = element->node->parent->data->viewport.y;
                    int viewportY2 = element->node->parent->data->viewport.y + element->node->parent->data->displaySize.h;
                    int thisX1 = element->position.x;
                    int thisX2 = element->position.x + element->displaySize.w;
                    int thisY1 = element->position.y;
                    int thisY2 = element->position.y + element->displaySize.h;
                    if (getAbsoluteValue<int>(thisX2+thisX1-viewportX2-viewportX1) <= (viewportX2-viewportX1+thisX2-thisX1) &&
                        getAbsoluteValue<int>(thisY2+thisY1-viewportY2-viewportY1) <= (viewportY2-viewportY1+thisY2-thisY1))
                    {                     
                        if (thisX1 >= viewportX1)
                        {
                            element->renderInfo.positionAndSize->x = thisX1 - viewportX1 + element->node->parent->data->position.x;
                            if (thisX2 > viewportX2)
                            {
                                element->renderInfo.positionAndSize->w = element->displaySize.w - thisX2 + viewportX2;
                                element->renderInfo.cropArea->x = 0;
                                element->renderInfo.cropArea->w = (static_cast<float>(element->renderInfo.positionAndSize->w) / static_cast<float>(element->displaySize.w)) * element->realSize.w;
                            }
                            else
                            {
                                element->renderInfo.positionAndSize->w = element->displaySize.w;
                                element->renderInfo.cropArea->x = 0;
                                element->renderInfo.cropArea->w = element->realSize.w;
                            }
                        }
                        else
                        {
                            element->renderInfo.positionAndSize->x = element->node->parent->data->position.x;
                            element->renderInfo.positionAndSize->w = thisX2 - viewportX1;
                            element->renderInfo.cropArea->x = (static_cast<float>(element->realSize.w) / static_cast<float>(element->displaySize.w)) * (element->displaySize.w - element->renderInfo.positionAndSize->w);
                            element->renderInfo.cropArea->w = (static_cast<float>(element->renderInfo.positionAndSize->w) / static_cast<float>(element->displaySize.w)) * element->realSize.w;
                        }

                        if (thisY1 >= viewportY1)
                        {
                            element->renderInfo.positionAndSize->y = thisY1 - viewportY1 + element->node->parent->data->position.y;
                            if (thisY2 > viewportY2)
                            {
                                element->renderInfo.positionAndSize->h = element->displaySize.h - thisY2 + viewportY2;
                                element->renderInfo.cropArea->y = 0;
                                element->renderInfo.cropArea->h = (static_cast<float>(element->renderInfo.positionAndSize->h) / static_cast<float>(element->displaySize.h)) * element->realSize.h;
                            }
                            else
                            {
                                element->renderInfo.positionAndSize->h = element->displaySize.h;
                                element->renderInfo.cropArea->y = 0;
                                element->renderInfo.cropArea->h = element->realSize.h;
                            }
                        }
                        else
                        {
                            element->renderInfo.positionAndSize->y = element->node->parent->data->position.y;
                            element->renderInfo.positionAndSize->h = thisY2 - viewportY1;
                            element->renderInfo.cropArea->y = (static_cast<float>(element->realSize.h) / static_cast<float>(element->displaySize.h)) * (element->displaySize.h - element->renderInfo.positionAndSize->h);
                            element->renderInfo.cropArea->h = (static_cast<float>(element->renderInfo.positionAndSize->h) / static_cast<float>(element->displaySize.h)) * element->realSize.h;
                        }
                    }
                    else
                    {
                        element->renderInfo.isRender = false;
                    }
                }
                break;
            }
            case KAKERA_POSREFER_WINDOW:
            {
                element->renderInfo.positionAndSize->x = element->position.x;
                element->renderInfo.positionAndSize->y = element->position.y;
                element->renderInfo.positionAndSize->w = element->displaySize.w;
                element->renderInfo.positionAndSize->h = element->displaySize.h;
                element->renderInfo.cropArea->x = 0;
                element->renderInfo.cropArea->y = 0;
                element->renderInfo.cropArea->w = element->realSize.w;
                element->renderInfo.cropArea->h = element->realSize.h;
                break;
            }
            default:
                break;
            }
            if (element->renderInfo.isRender && element->texture != nullptr)
            {
                SDL_RenderCopyEx(
                    window->renderer,
                    element->texture,
                    element->renderInfo.cropArea,
                    element->renderInfo.positionAndSize,
                    element->rotateAngle,
                    NULL,
                    SDL_FLIP_NONE
                );
            }
        }
    }
    SDL_RenderPresent(window->renderer);
}

void kakera_StartWindow(kakera_Window * window)
{
    SDL_Event event;
    SDL_SetEventFilter(kakera_private_EventFilter, window);
    while (!window->isQuit)
    {
        SDL_TimerID FPSTimer = SDL_AddTimer(1000 / window->FPS, kakera_private_FPSSemCallback, window);
        kakera_pirvate_RefreshFrame(window);
        SDL_PollEvent(&event);
        SDL_SemWait(window->FPSSem);
        SDL_RemoveTimer(FPSTimer);
        kakera_pirvate_RefreshFrame(window);
    }
    if (window->isQuit)
    {
        kakera_DestroyWindow(window);
        return;
    }
}
