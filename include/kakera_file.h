#ifndef KAKERA_CORE_FILE
#define KAKERA_CORE_FILE

#ifdef __cplusplus
extern "C"
{
#endif

#include "kakera_main.h"
#include "kakera_declaration.h"

typedef kakera_File*(*kakera_CryptMethod)(kakera_File*);

extern KAKERA_EXPORT kakera_File*           kakera_ReadFile(const char* filepath);
extern KAKERA_EXPORT kakera_File*           kakera_CreateNewFile(int size, char* data);
extern KAKERA_EXPORT const char* const      kakera_GetRawFileData(kakera_File* file);
extern KAKERA_EXPORT int                    kakera_GetRawFileSize(kakera_File* file);
extern KAKERA_EXPORT void                   kakera_DestroyFile(kakera_File** file);
extern KAKERA_EXPORT kakera_FilePackage*    kakera_CreateNewFilePackage(const char* path);
extern KAKERA_EXPORT kakera_FilePackage*    kakera_LoadFilePackage(const char* path);
extern KAKERA_EXPORT void                   kakera_DestroyFilePackage(kakera_FilePackage* package);
extern KAKERA_EXPORT kakera_File*           kakera_ReadFileFromPackage(kakera_FilePackage* package, const char* path);
extern KAKERA_EXPORT void                   kakera_AddFileToPackage(kakera_FilePackage* package, kakera_File* file, const char* path);
extern KAKERA_EXPORT void                   kakera_SetPackageEncryptMethod(kakera_FilePackage* package, kakera_CryptMethod method);
extern KAKERA_EXPORT void                   kakera_SetPackageDecryptMethod(kakera_FilePackage* package, kakera_CryptMethod method);

#ifdef __cplusplus
}
#endif

#endif // !KAKERA_CORE_FILE
