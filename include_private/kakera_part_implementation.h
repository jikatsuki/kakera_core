#ifndef KAKERA_CORE_PART_IMPLEMENTATION
#define KAKERA_CORE_PART_IMPLEMENTATION

#include "kakera_declaration.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_window.h"
#include "kakera_scene.h"
#include "kakera_element.h"
#include "kakera_event.h"
#include "kakera_file.h"
#include <map>
#include <string>
#include <cstdlib>
#include "pugixml/pugixml.hpp"

using namespace std;
using namespace kakera_private;

struct kakera_Window
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_semaphore* FPSSem;
    bool isQuit;
    map<const char*, kakera_Scene*> sceneList;
    kakera_Scene* activeScene = nullptr;
    kakera_WindowFPS FPS = KAKERA_WINDOW_30FPS;
    kakera_Event event;
    void* userdata = nullptr;
    bool usingDirtyRect = false;
};

struct kakera_Scene
{
    kakera_Window* window = nullptr;
    Tree<kakera_Element*> elementList;
    map<kakera_SceneEvents, kakera_SceneEventCallback> callbackList;
    kakera_Element* mouseEnteredElement = nullptr;
    kakera_Element* focusElement = nullptr;
    void* userdata = nullptr;
};

struct kakera_Element
{
    kakera_Scene* scene = nullptr;
    const char* name;
    SDL_Texture* texture = nullptr;
    Tree<kakera_Element*>::Node* node;
    bool isResponseEvent = true;
    map<kakera_ElementEvents, kakera_ElementEventCallback> callbackList;
    Position_2D position = { 0, 0 };
    Position_2D viewport = { 0, 0 };
    Size_2D displaySize = { 0, 0 };
    Size_2D realSize = { 0, 0 };
    bool resizeFlag = false;
    double rotateAngle = 0.0;
    int SDLFormat;
    int SDLAccess;
    kakera_PositionReference reference;
    RenderInfo renderInfo;
    bool isMouseEntered = false;
    map<const char*, void*> dataList;
    bool isReceiveInput = false;
    string receivedInput;
    string receivedEditingInput;
};

struct kakera_FilePackage
{
    FILE* data;
    pugi::xml_document index;
    const char* path;
    kakera_CryptMethod encrypt = nullptr;
    kakera_CryptMethod decrypt = nullptr;
};

#endif //!KAKERA_CORE_PART_IMPLEMENTATION