#ifndef KAKERA_CORE_EVENT
#define KAKERA_CORE_EVENT

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"

typedef enum {
    KAKERA_SCENE_ON_CREATE,
    KAKERA_SCENE_ON_DESTROY,
    KAKERA_SCENE_ON_START,
    KAKERA_SCENE_ON_STOP
} kakera_SceneEvents;

typedef void(*kakera_SceneEventCallback)(kakera_Scene*);

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

typedef void(*kakera_ElementEventCallback)(kakera_Element*);

typedef enum {
    KAKERA_MOUSE_LEFT_BUTTON,
    KAKERA_MOUSE_RIGHT_BUTTON,
    KAKERA_MOUSE_MIDDLE_BUTTON
} kakera_Mouse_Button;

typedef enum {
    KAKERA_MOUSE_BUTTON_PRESSED,
    KAKERA_MOUSE_BUTTON_RELEASED
} kakera_Mouse_Button_Status;

typedef enum {
    KAKERA_KEY_ESC = 41,
    KAKERA_KEY_F1 = 58,
    KAKERA_KEY_F2 = 59,
    KAKERA_KEY_F3 = 60,
    KAKERA_KEY_F4 = 61,
    KAKERA_KEY_F5 = 62,
    KAKERA_KEY_F6 = 63,
    KAKERA_KEY_F7 = 64,
    KAKERA_KEY_F8 = 65,
    KAKERA_KEY_F9 = 66,
    KAKERA_KEY_F10 = 67,
    KAKERA_KEY_F11 = 68,
    KAKERA_KEY_F12 = 69,
    KAKERA_KEY_PRINT_SCREEN = 70,
    KAKERA_KEY_SCROLL_LOCK = 71,
    KAKERA_KEY_PAUSE = 72,
    KAKERA_KEY_INSERT = 73,
    KAKERA_KEY_HOME = 74,
    KAKERA_KEY_PAGE_UP = 75,
    KAKERA_KEY_PAGE_DOWN = 78,
    KAKERA_KEY_DELETE = 76,
    KAKERA_KEY_END = 77,
    KAKERA_KEY_TAB = 43,
    KAKERA_KEY_CAPS_LOCK = 57,
    KAKERA_KEY_ENTER = 40,
    KAKERA_KEY_LEFT_ALT = 226,
    KAKERA_KEY_RIGHT_ALT = 230,
    KAKERA_KEY_LEFT_SHIFT = 225,
    KAKERA_KEY_RIGHT_SHIFT = 229,
    KAKERA_KEY_LEFT_CTRL = 224,
    KAKERA_KEY_RIGHT_CTRL = 228,
    KAKERA_KEY_ARROW_UP = 82,
    KAKERA_KEY_ARROW_DOWN = 81,
    KAKERA_KEY_ARROW_LEFT = 80,
    KAKERA_KEY_ARROW_RIGHT = 79,
    KAKERA_KEY_0 = 39,
    KAKERA_KEY_1 = 30,
    KAKERA_KEY_2 = 31,
    KAKERA_KEY_3 = 32,
    KAKERA_KEY_4 = 33,
    KAKERA_KEY_5 = 34,
    KAKERA_KEY_6 = 35,
    KAKERA_KEY_7 = 36,
    KAKERA_KEY_8 = 37,
    KAKERA_KEY_9 = 38,
    KAKERA_KEY_A = 4,
    KAKERA_KEY_B = 5,
    KAKERA_KEY_C = 6,
    KAKERA_KEY_D = 7,
    KAKERA_KEY_E = 8,
    KAKERA_KEY_F = 9,
    KAKERA_KEY_G = 10,
    KAKERA_KEY_H = 11,
    KAKERA_KEY_I = 12,
    KAKERA_KEY_J = 13,
    KAKERA_KEY_K = 14,
    KAKERA_KEY_L = 15,
    KAKERA_KEY_M = 16,
    KAKERA_KEY_N = 17,
    KAKERA_KEY_O = 18,
    KAKERA_KEY_P = 19,
    KAKERA_KEY_Q = 20,
    KAKERA_KEY_R = 21,
    KAKERA_KEY_S = 22,
    KAKERA_KEY_T = 23,
    KAKERA_KEY_U = 24,
    KAKERA_KEY_V = 25,
    KAKERA_KEY_W = 26,
    KAKERA_KEY_X = 27,
    KAKERA_KEY_Y = 28,
    KAKERA_KEY_Z = 29,
    KAKERA_KEY_MINUS = 45,
    KAKERA_KEY_EQUALS = 46,
    KAKERA_KEY_LEFT_BRACKET = 47,
    KAKERA_KEY_RIGHT_BRACKET = 48,
    KAKERA_KEY_BACK_SLASH = 49,
    KAKERA_KEY_SEMICOLON = 51,
    KAKERA_KEY_QUOTE = 52,
    KAKERA_KEY_COMMA = 54,
    KAKERA_KEY_PERIOD = 55,
    KAKERA_KEY_SLASH = 56,
    KAKERA_KEY_BACK_QUOTE = 53,
    KAKERA_KEY_SPACE = 44,
    KAKERA_KEY_BACKSPACE = 42,
    KAKERA_KEY_APPLICATION = 101,
    KAKERA_KEY_WIN = 101
} kakera_Keyboard_Key;

typedef struct {
    kakera_Mouse_Button button;
    kakera_Mouse_Button_Status status;
    int x, y;
} kakera_Mouse_Event;

typedef struct {
    kakera_Keyboard_Key key;
} kakera_Keyboard_Event;

typedef struct {
} kakera_Wheel_Event;

typedef union {
    //この歌声がUNION!!!
    kakera_Mouse_Event mouse;
    kakera_Keyboard_Event keyboard;
    kakera_Wheel_Event wheel;
} kakera_Event;

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_EVENT
