#include "kakera_element.h"

using namespace std;

struct kakera_Element
{
    const char* name;
};

kakera_Element * kakera_CreateElement(const char * id)
{
    return nullptr;
}

void kakera_DestroyElement(kakera_Element * element)
{
    
}

const char * kakera_GetElementName(kakera_Element * element)
{
    return element->name;
}

void kakera_SetIsElementResponseEvent(kakera_Element * element, bool response)
{
    
}

void kakera_BindEvedntToElement(kakera_Element * element, kakera_ElementEvents event, kakera_ElementEventCallback callback)
{
    
}
