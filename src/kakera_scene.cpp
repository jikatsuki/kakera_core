#include "kakera_scene.h"
#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_element.h"
#include <map>

using namespace std;

struct kakera_Scene
{
    kakera_Window* window;
    Tree<kakera_Element*> elementList;
    map<kakera_SceneEvents, kakera_SceneEventCallback> callbackList;
    const char* name;
};

kakera_Scene * kakera_CreateScene(const char * name)
{
    kakera_Scene* result = new kakera_Scene;
    result->name = name;
    return result;
}

void kakera_DestroyScene(kakera_Scene * scene)
{
    scene->callbackList[KAKERA_SCENE_ON_DESTROY](scene);
    scene->elementList.Clear();
    scene->callbackList.clear();
    delete scene;
}

const char * kakera_GetSceneName(kakera_Scene* scene)
{
    return scene->name;
}

void kakera_BindSceneWithWindow(kakera_Scene * scene, kakera_Window * window)
{
    
}

void kakera_AddElementToScene(kakera_Scene * scene, kakera_Element * element, kakera_Element * parent)
{
    Tree<kakera_Element*>::Node* elementNode = new Tree<kakera_Element*>::Node;
    elementNode->data = element;
    Tree<kakera_Element*>::Node* parentNode;
    if (parent != nullptr)
    {
        parentNode = scene->elementList.GetNodeByData(parent);
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
    scene->elementList.InsertNode(elementNode, parentNode);
}

kakera_Element * kakera_GetElementByIDFromScene(kakera_Scene * scene, const char * id)
{
    kakera_Element* result = nullptr;
    scene->elementList.BreadthFirstSearch([&id, &result](Tree<kakera_Element*>::Node* node) {
        if (id == kakera_GetElementName(node->data))
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
    auto iter = scene->callbackList.find(event);
    if (iter == scene->callbackList.end())
    {
        scene->callbackList.emplace(event, callback);
    }
    else
    {
        scene->callbackList[event] = callback;
    }
}
