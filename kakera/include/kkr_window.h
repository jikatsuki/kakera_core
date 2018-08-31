#ifndef __KKR_WINDOW_H__
#define __KKR_WINDOW_H__

#include "kkr_header.h"
#include "kkr_struct.hpp"

namespace kakera
{
    class KKR_EXPORT Window
    {
    public:
        bool isQuit = false;

        Window();
        Window(const Window &other);
        Window(Window &&other);
        Window(const char *title, int x, int y, int width, int height);

        void Init(const char *title, int x, int y, int width, int height);

        const char * GetTitle();
        void SetTitle(const char *title);

        Point GetPosition();
        void SetPosition(Point point);

        Size GetSize();
        void SetSize(Size size);

        Uint32 GetSDLWindowID();

        int Start();
    private:
        SDL_Window *_SDLWindow;
        SDL_Renderer *_SDLRenderer;

        int _MainLoop();
        void _Terminate();
    };
}

#endif // !__KKR_WINDOW_H__
