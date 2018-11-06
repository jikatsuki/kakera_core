#include "kakera_scene.h"
#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_element.h"
#include "kakera_part_implementation.h"
#include "kakera_tools.hpp"
#include <forward_list>
#include <map>
#include <mutex>

KAKERA_PRIVATE_USING_REFRESH_EVENT

using namespace std;
using namespace kakera_private;

kakera_Scene * kakera_CreateScene()
{
    auto result = new kakera_Scene;
    return result;
}

void kakera_InitializeScene(kakera_Scene* scene)
{
    kakera_private_CheckNullPointer(scene);
    kakera_private_RunCallback(scene, KAKERA_SCENE_ON_CREATE);
}

void kakera_DestroyScene(kakera_Scene ** scene)
{
    kakera_private_CheckNullPointer(*scene);
    kakera_private_RunCallback((*scene), KAKERA_SCENE_ON_DESTROY);
    forward_list<kakera_Element*> elements;
    (*scene)->elementList.BreadthFirstSearch([&elements](Tree<kakera_Element*>::Node* node) {
        elements.emplace_front(node->data);
    });
    for (auto element : elements)
    {
        kakera_DestroyElement(&element);
    }
    (*scene)->callbackList.clear();
    delete *scene;
    *scene = nullptr;
}

void kakera_BindSceneWithWindow(kakera_Scene * scene, kakera_Window * window)
{
    kakera_private_CheckNullPointer(scene);
    kakera_private_CheckNullPointer(window);
    scene->window = window;
}

void kakera_AddElementToScene(kakera_Scene * scene, kakera_Element * element, kakera_Element * parent)
{
    kakera_private_CheckNullPointer(scene);
    kakera_private_CheckNullPointer(element);
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
    kakera_private_CheckNullPointer(scene);
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
    kakera_private_CheckNullPointer(scene);
    kakera_private_CheckNullPointer(element);
    Tree<kakera_Element*>::Node* elementNode = scene->elementList.GetNodeByData(element);
    if (elementNode != nullptr)
    {
        scene->elementList.DeleteNode(elementNode);
    }
}

void kakera_BindEventToScene(kakera_Scene * scene, kakera_SceneEvents event, kakera_SceneEventCallback callback)
{
    kakera_private_CheckNullPointer(scene);
    scene->callbackList.emplace(event, callback);
}

void kakera_SetFocusElement(kakera_Scene * scene, kakera_Element * element)
{
    kakera_private_CheckNullPointer(scene);
    kakera_private_CheckNullPointer(element);
    if (scene->focusElement != element)
    {
        if (scene->focusElement != nullptr)
        {
            kakera_private_RunCallback(scene->focusElement, KAKERA_ELEMENT_ON_BLUR);
        }
        scene->focusElement = element;
        kakera_private_RunCallback(scene->focusElement, KAKERA_ELEMENT_ON_FOCUS);
    }
}

kakera_Window * kakera_GetWindowFromScene(kakera_Scene * scene)
{
    kakera_private_CheckNullPointer(scene);
    return scene->window;
}

void * kakera_GetSceneUserdata(kakera_Scene * scene)
{
    kakera_private_CheckNullPointer(scene);
    return scene->userdata;
}

void kakera_StartScene(kakera_Scene* scene, void* userdata)
{
    kakera_private_CheckNullPointer(scene);
    kakera_private_CheckNullPointer(scene->window);
    scene->userdata = userdata;
    auto lock = &scene->window->eventLock;
    if (scene->window->activeScene != nullptr)
    {
        kakera_private_RunCallbackAsync(scene->window->activeScene, KAKERA_SCENE_ON_STOP, lock);
    }
    scene->window->activeScene = scene;
    kakera_private_RunCallbackAsync(scene, KAKERA_SCENE_ON_START, lock);
    kakera_private::PushRefreshEvent();
}
