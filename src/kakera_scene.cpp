#include "kakera_scene.h"
#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_element.h"
#include "kakera_part_implementation.h"
#include <map>

using namespace std;

kakera_Scene * kakera_CreateScene()
{
    auto result = new kakera_Scene;
    return result;
}

void kakera_InitializeScene(kakera_Scene* scene)
{
    kakera_RunCallback(scene, KAKERA_SCENE_ON_CREATE);
}

void kakera_DestroyScene(kakera_Scene * scene)
{
    kakera_RunCallback(scene, KAKERA_SCENE_ON_DESTROY);
    scene->elementList.Clear();
    scene->callbackList.clear();
    delete scene;
}

void kakera_BindSceneWithWindow(kakera_Scene * scene, kakera_Window * window)
{
    scene->window = window;
}

void kakera_AddElementToScene(kakera_Scene * scene, kakera_Element * element, kakera_Element * parent)
{
    Tree<kakera_Element*>::Node* parentNode;
    if (parent != nullptr)
    {
        parentNode = parent->node;
    }
    else
    {
        if (scene->elementList.GetRoot() != nullptr)
        {
            parentNode = scene->elementList.GetRoot();
        }
        else
        {
            parentNode = nullptr;
        }
    }
    scene->elementList.InsertNode(element->node, parentNode);
    element->scene = scene;
}

kakera_Element * kakera_GetElementByNameFromScene(kakera_Scene * scene, const char * name)
{
    kakera_Element* result = nullptr;
    scene->elementList.BreadthFirstSearch([&name, &result](Tree<kakera_Element*>::Node* node) {
        if (name == kakera_GetElementName(node->data))
        {
            result = node->data;
        }
    });
    return result;
}

void kakera_DeleteElementFromScene(kakera_Scene * scene, kakera_Element * element)
{
    Tree<kakera_Element*>::Node* elementNode = scene->elementList.GetNodeByData(element);
    if (elementNode != nullptr)
    {
        scene->elementList.DeleteNode(elementNode);
    }
}

void kakera_BindEventToScene(kakera_Scene * scene, kakera_SceneEvents event, kakera_SceneEventCallback callback)
{
    scene->callbackList.emplace(event, callback);
}

void kakera_StartScene(kakera_Window * window, kakera_Scene * scene)
{
    if (window->activeScene != nullptr)
    {
        kakera_RunCallback(window->activeScene, KAKERA_SCENE_ON_STOP);
    }
    window->activeScene = scene;
    kakera_RunCallback(scene, KAKERA_SCENE_ON_START);
}
