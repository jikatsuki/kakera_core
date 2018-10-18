#include "kakera_header.h"
#include "kakera_element.h"
#include "kakera_part_implementation.h"

using namespace std;

kakera_Element * kakera_CreateElement(const char * id)
{
    kakera_Element* result = new kakera_Element;
    return result;;
}

void kakera_DestroyElement(kakera_Element * element)
{
    delete element;
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
