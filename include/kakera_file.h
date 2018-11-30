#ifndef KAKERA_CORE_FILE
#define KAKERA_CORE_FILE

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"

extern KAKERA_EXPORT kakera_File*           kakera_ReadFile(const char* filepath);
extern KAKERA_EXPORT kakera_File*           kakera_CreateNewFile(int size, char* data);
extern KAKERA_EXPORT const char* const      kakera_GetRawFileData(kakera_File* file);
extern KAKERA_EXPORT int                    kakera_GetRawFileSize(kakera_File* file);
extern KAKERA_EXPORT void                   kakera_DestroyFile(kakera_File** file);

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_FILE
