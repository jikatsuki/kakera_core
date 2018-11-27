#include "kakera_scene.h"
#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_element.h"
#include "kakera_part_implementation.h"
#include "kakera_tools.hpp"
#include "kakera_private_apis.h"
#include <forward_list>
#include <map>
#include <mutex>
#include <future>

using namespace std;
using namespace kakera_private;

kakera_Scene * kakera_CreateScene()
{
    auto result = new kakera_Scene;
    return result;
}

void kakera_InitializeScene(kakera_Scene* scene)
{
    kakera_private::CheckNullPointer(scene);
    kakera_private::RunCallback(scene, KAKERA_SCENE_ON_CREATE);
}

void kakera_DestroyScene(kakera_Scene ** scene)
{
    kakera_private::CheckNullPointer(*scene);
    kakera_private::RunCallback((*scene), KAKERA_SCENE_ON_DESTROY);
    (*scene)->elementTree.Clear();
    for (auto iter = (*scene)->elementList->rbegin(); iter != (*scene)->elementList->rend(); iter++)
    {
        auto element = *iter;
        kakera_private::DestroyElementWithoutRefreshingList(&element);
    }
    
    (*scene)->callbackList.clear();
    delete (*scene)->elementList;
    delete *scene;
    *scene = nullptr;
}

void kakera_BindSceneWithWindow(kakera_Scene * scene, kakera_Window * window)
{
    kakera_private::CheckNullPointer(scene);
    kakera_private::CheckNullPointer(window);
    scene->window = window;
}

void kakera_AddElementToScene(kakera_Scene * scene, kakera_Element * element, kakera_Element * parent)
{
    kakera_private::CheckNullPointer(scene);
    kakera_private::CheckNullPointer(element);
    Tree<kakera_Element*>::Node* parentNode;
    if (parent != nullptr)
    {
        parentNode = parent->node;
    }
    else
    {
        if (scene->elementTree.GetRoot() != nullptr)
        {
            parentNode = scene->elementTree.GetRoot();
        }
        else
        {
            parentNode = nullptr;
        }
    }
    scene->elementTree.InsertNode(element->node, parentNode);
    delete scene->elementList;
    scene->elementList = scene->elementTree.ConvertToList();
    element->scene = scene;
}

kakera_Element * kakera_GetElementByNameFromScene(kakera_Scene * scene, const char * name)
{
    kakera_private::CheckNullPointer(scene);
    kakera_Element* result = nullptr;
    scene->elementTree.BreadthFirstSearch([&name, &result](Tree<kakera_Element*>::Node* node) {
        if (name == kakera_GetElementName(node->data))
        {
            result = node->data;
        }
    });
    return result;
}

void kakera_DeleteElementFromScene(kakera_Scene * scene, kakera_Element * element)
{
    kakera_private::CheckNullPointer(scene);
    kakera_private::CheckNullPointer(element);
    Tree<kakera_Element*>::Node* elementNode = scene->elementTree.GetNodeByData(element);
    if (elementNode != nullptr)
    {
        delete element;
        scene->elementTree.DeleteNode(elementNode);
    }
    delete scene->elementList;
    scene->elementList = scene->elementTree.ConvertToList();
}

void kakera_BindEventToScene(kakera_Scene * scene, kakera_SceneEvents event, kakera_SceneEventCallback callback)
{
    kakera_private::CheckNullPointer(scene);
    scene->callbackList.emplace(event, callback);
}

void kakera_SetFocusElement(kakera_Scene * scene, kakera_Element * element)
{
    kakera_private::CheckNullPointer(scene);
    kakera_private::CheckNullPointer(element);
    if (scene->focusElement != element)
    {
        if (scene->focusElement != nullptr)
        {
            kakera_private::RunCallback(scene->focusElement, KAKERA_ELEMENT_ON_BLUR);
        }
        scene->focusElement = element;
        kakera_private::RunCallback(scene->focusElement, KAKERA_ELEMENT_ON_FOCUS);
    }
}

kakera_Window * kakera_GetWindowFromScene(kakera_Scene * scene)
{
    kakera_private::CheckNullPointer(scene);
    return scene->window;
}

void * kakera_GetSceneUserdata(kakera_Scene * scene)
{
    kakera_private::CheckNullPointer(scene);
    return scene->userdata;
}

void kakera_StartScene(kakera_Scene* scene, void* userdata)
{
    kakera_private::CheckNullPointer(scene);
    kakera_private::CheckNullPointer(scene->window);
    scene->userdata = userdata;
    if (scene->window->activeScene != nullptr)
    {
        kakera_private::RunCallback(scene->window->activeScene, KAKERA_SCENE_ON_STOP);
    }
    scene->window->activeScene = scene;
    kakera_private::RunCallback(scene, KAKERA_SCENE_ON_START);
    kakera_private::PushRefreshEvent(scene);
}
