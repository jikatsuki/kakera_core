#ifndef KAKERA_CORE_DECLARATION
#define KAKERA_CORE_DECLARATION

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct kakera_Window;
typedef struct kakera_Scene;
typedef struct kakera_Element;
typedef struct kakera_FilePackage;

typedef struct {
    int size;
    char* data;
} kakera_File;

typedef struct {
    int x, y, w, h;
} kakera_Rectangle;

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_DECLARATION