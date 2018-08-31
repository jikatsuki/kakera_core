#include <cstdlib>

#define KAKERA_IMPORT
#include "kakera.h"

using namespace std;

int main(int argc, char *argv[])
{
    kakera::Window window;
    window.Init("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900);
    window.Start();
    return 0;
}