#ifndef KAKERA_CORE_PRIVATE_APIS
#define KAKERA_CORE_PRIVATE_APIS

#include "kakera_declaration.h"
#include "kakera_part_implementation.h"
#include "kakera_structs.hpp"
#include "kakera_header.h"

void kakera_BindSceneWithWindow(kakera_Scene* scene, kakera_Window* window)
{
    scene->window = window;
}

void kakera_SetNodeForElement(kakera_Element* element, Tree<kakera_Element*>::Node* node)
{
    element->node = node;
}

SDL_Texture* kakera_GetElementTexture(kakera_Element* element)
{
    return element->texture;
}

#endif // !KAKERA_CORE_PRIVATE_APIS
