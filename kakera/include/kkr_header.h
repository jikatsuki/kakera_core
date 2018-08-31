#ifndef __KKR_HEADER_H__
#define __KKR_HEADER_H__

#ifdef KAKERA_IMPORT
#define KKR_EXPORT __declspec(dllimport)
#else
#define KKR_EXPORT __declspec(dllexport)
#endif // !KAKERA_IMPORT

#include <iostream>
#include <cstdlib>

extern "C"
{
#include "SDL/SDL.h"
#undef main
}

#include "pugixml/pugixml.hpp"

namespace kakera
{
    class Window;
    class Scene;
    class Element;
}

#endif // !__KKR_HEADER_H__
