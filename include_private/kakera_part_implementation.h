#ifndef KAKERA_CORE_PART_IMPLEMENTATION
#define KAKERA_CORE_PART_IMPLEMENTATION

#include "kakera_declaration.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_scene.h"
#include "kakera_element.h"
#include <map>

using namespace std;

struct kakera_Window
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_semaphore* FPSSem;
    bool isQuit;
    map<const char*, kakera_Scene*> sceneList;
    kakera_Scene* activeScene = nullptr;
};

struct kakera_Scene
{
    kakera_Window* window;
    Tree<kakera_Element*> elementList;
    map<kakera_SceneEvents, kakera_SceneEventCallback> callbackList;
};

struct kakera_Element
{
    kakera_Scene* scene;
    const char* name;
    SDL_Texture* texture;
    Tree<kakera_Element*>::Node* node;
    bool isResponseEvent = true;
    map<kakera_ElementEvents, kakera_ElementEventCallback> callbackList;
    Position_2D position;
    Position_2D viewport;
    Size_2D displaySize;
    Size_2D realSize;
    double rotateAngle = 0.0;
};

struct kakera_File
{
    int size;
    char* data;
};

#endif //!KAKERA_CORE_PART_IMPLEMENTATION