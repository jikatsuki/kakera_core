#ifndef KAKERA_CORE_WINDOW
#define KAKERA_CORE_WINDOW

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "kakera_main.h"
#include "kakera_scene.h"
#include "kakera_declaration.h"
#include "kakera_file.h"
#include "kakera_event.h"

typedef enum {
    KAKERA_WINDOW_FULLSCREEN            = 1,
    KAKERA_WINDOW_SHOWN                 = 4,
    KAKERA_WINDOW_HIDDEN                = 8,
    KAKERA_WINDOW_BORDERLESS            = 16,
    KAKERA_WINDOW_RESIZABLE             = 32,
    KAKERA_WINDOW_MINIMIZED             = 64,
    KAKERA_WINDOW_MAXIMIZED             = 128,
    KAKERA_WINDOW_FULLSCREEN_DESKTOP    = 4097
} kakera_WindowFlags;

typedef enum {
    KAKERA_WINDOW_30FPS = 30,
    KAKERA_WINDOW_60FPS = 60
} kakera_WindowFPS;

extern KAKERA_EXPORT unsigned int       kakera_GetWindowPosCentered();
extern KAKERA_EXPORT kakera_Window*     kakera_CreateWindow(const char* title, int x, int y, int w, int h, int flags);
extern KAKERA_EXPORT int                kakera_DestroyWindow(kakera_Window** window);
extern KAKERA_EXPORT void               kakera_GetWindowSize(kakera_Window* window, int* w, int* h);
extern KAKERA_EXPORT void               kakera_SetWindowSize(kakera_Window* window, int w, int h);
extern KAKERA_EXPORT void               kakera_GetWindowPosition(kakera_Window* window, int* x, int* y);
extern KAKERA_EXPORT void               kakera_SetWindowPosition(kakera_Window* window, int x, int y);
extern KAKERA_EXPORT const char*        kakera_GetWindowTitle(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_SetWindowTitle(kakera_Window* window, const char* title);
extern KAKERA_EXPORT void*              kakera_GetWindowUserdata(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_ShowWindow(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_HideWindow(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_MinimizeWindow(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_MaximizeWindow(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_RaiseWindow(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_SetWindowResizeable(kakera_Window* window, bool resizeable);
extern KAKERA_EXPORT void               kakera_SetWindowIcon(kakera_Window* window, kakera_File* icon);
extern KAKERA_EXPORT kakera_WindowFPS   kakera_GetWindowFPS(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_SetWindowFPS(kakera_Window* window, kakera_WindowFPS FPS);
extern KAKERA_EXPORT kakera_Event*      kakera_GetWindowEvent(kakera_Window* window);
extern KAKERA_EXPORT void               kakera_SetUsingDirtyRectRender(kakera_Window* window, bool isUse);
extern KAKERA_EXPORT void               kakera_StartWindow(kakera_Window** window, void* userdata);

#ifdef __cplusplus
}
#endif

#endif //!KAKERA_CORE_WINDOW