#include "kakera_scene.h"
#include "kakera_window.h"
#include "kakera_header.h"
#include "kakera_structs.hpp"
#include "kakera_element.h"

using namespace std;

struct kakera_Scene
{
    kakera_Window* window;
    Tree<kakera_Element> elementList;
    const char* name;
};