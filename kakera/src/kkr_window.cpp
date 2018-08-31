#include "kkr_window.h"

kakera::Window::Window()
{
    _SDLWindow = nullptr;
    _SDLRenderer = nullptr;
}

kakera::Window::Window(const Window & other)
{
    _SDLWindow = other._SDLWindow;
    _SDLRenderer = other._SDLRenderer;
}

kakera::Window::Window(Window && other)
{
    _SDLWindow = std::move(other._SDLWindow);
    _SDLRenderer = std::move(other._SDLRenderer);
}

kakera::Window::Window(const char * title, int x, int y, int width, int height)
{
    Init(title, x, y, width, height);
}

void kakera::Window::Init(const char * title, int x, int y, int width, int height)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    _SDLWindow = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    _SDLRenderer = SDL_CreateRenderer(_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
}

const char * kakera::Window::GetTitle()
{
    return SDL_GetWindowTitle(_SDLWindow);
}

void kakera::Window::SetTitle(const char * title)
{
    SDL_SetWindowTitle(_SDLWindow, title);
}

kakera::Point kakera::Window::GetPosition()
{
    Point result;
    SDL_GetWindowPosition(_SDLWindow, &result.x, &result.y);
    return result;
}

void kakera::Window::SetPosition(Point point)
{
    SDL_SetWindowPosition(_SDLWindow, point.x, point.y);
}

kakera::Size kakera::Window::GetSize()
{
    Size result;
    SDL_GetWindowSize(_SDLWindow, &result.width, &result.height);
    return result;
}

void kakera::Window::SetSize(Size size)
{
    SDL_SetWindowSize(_SDLWindow, size.width, size.height);
}

Uint32 kakera::Window::GetSDLWindowID()
{
    return SDL_GetWindowID(_SDLWindow);
}

int kakera::Window::Start()
{
    SDL_Event event;

    while (!isQuit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    isQuit = true;
                    break;
                }
            }
        }
        _MainLoop();
    }

    if (isQuit)
    {
        _Terminate();
    }
    return 0;
}

void kakera::Window::_Terminate()
{
    
}

int kakera::Window::_MainLoop()
{
    return 0;
}
