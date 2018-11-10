#ifndef KAKERA_CORE_DECLARATION
#define KAKERA_CORE_DECLARATION

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct kakera_Window;
typedef struct kakera_Scene;
typedef struct kakera_Element;
typedef struct {
    int size;
    char* data;
} kakera_File;
typedef struct kakera_FilePackage;

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_DECLARATION