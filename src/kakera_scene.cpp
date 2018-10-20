#include "kakera_scene.h"
#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_element.h"
#include "kakera_part_implementation.h"
#include <map>

using namespace std;

kakera_Scene * kakera_CreateScene(const char * name)
{
    kakera_Scene* result = new kakera_Scene;
    result->name = name;
    //kakera_RunCallback(result, KAKERA_SCENE_ON_CREATE);
    return result;
}

void kakera_DestroyScene(kakera_Scene * scene)
{
    kakera_RunCallback(scene, KAKERA_SCENE_ON_DESTROY);
    scene->elementList.Clear();
    scene->callbackList.clear();
    delete scene;
}

const char * kakera_GetSceneName(kakera_Scene* scene)
{
    return scene->name;
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
    kakera_RunCallback(element, KAKERA_ELEMENT_ON_CREATE);
    scene->elementList.InsertNode(element->node, parentNode);
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
    scene->callbackList.emplace(event, callback);
}
