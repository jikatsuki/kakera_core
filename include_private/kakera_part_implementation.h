#ifndef KAKERA_CORE_PART_IMPLEMENTATION
#define KAKERA_CORE_PART_IMPLEMENTATION

#include "kakera_declaration.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_scene.h"
#include <map>

using namespace std;

struct kakera_Window
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_semaphore* FPSSem;
    bool isQuit;
    map<const char*, kakera_Scene*> sceneList;
};

struct kakera_Scene
{
    kakera_Window* window;
    Tree<kakera_Element*> elementList;
    map<kakera_SceneEvents, kakera_SceneEventCallback> callbackList;
    const char* name;
};

struct kakera_Element
{
    const char* name;
    SDL_Texture* texture;
    Tree<kakera_Element*>::Node* node;
};

#endif //!KAKERA_CORE_PART_IMPLEMENTATION