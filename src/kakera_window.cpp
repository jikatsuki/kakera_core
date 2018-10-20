#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_part_implementation.h"

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

void kakera_RegisterScene(kakera_Window * window, const char* name, kakera_Scene * scene)
{
    auto iter = window->sceneList.find(name);
    if (iter == window->sceneList.end())
    {
        window->sceneList.emplace(name, scene);
    }
    else
    {
        window->sceneList[name] = scene;
    }
    scene->window = window;
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

void kakera_SetWindowIcon(kakera_Window * window)
{
    //SDL_SetWindowIcon();
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

    SDL_RenderPresent(window->renderer);
}

void kakera_StartWindow(kakera_Window * window)
{
    SDL_Event event;
    SDL_SetEventFilter(kakera_private_EventFilter, window);
    while (!window->isQuit)
    {
        SDL_TimerID FPSTimer = SDL_AddTimer(1000 / 60, kakera_private_FPSSemCallback, window);
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
