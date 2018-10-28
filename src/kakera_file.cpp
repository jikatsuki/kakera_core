#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //!_MSC_VER

#include "kakera_file.h"
#include "kakera_part_implementation.h"
#include <cstdio>

using namespace std;

kakera_File * kakera_ReadFile(const char * filepath)
{
    FILE* file = fopen(filepath, "rb");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    char* data = new char[size];
    fseek(file, 0, SEEK_SET);
    fread(data, sizeof(char), size, file);
    fclose(file);
    kakera_File* result = new kakera_File;
    result->data = data;
    result->size = size;
    return result;
}

kakera_File * kakera_CreateNewFile(int size, char * data)
{
    kakera_File* result = new kakera_File;
    result->data = data;
    result->size = size;
    return result;
}

void kakera_DestroyFile(kakera_File * file)
{
    delete file->data;
    delete file;
}

kakera_FilePackage * kakera_CreateNewFilePackage()
{
    return nullptr;
}

kakera_FilePackage * kakera_LoadFilePackage(kakera_File * packageFile)
{
    return nullptr;
}

void kakera_DestroyFilePackage(kakera_FilePackage * package)
{
    
}

kakera_File * kakera_ReadFileFromPackage(kakera_FilePackage * package, const char * path)
{
    return nullptr;
}

void kakera_AddFileToPackage(kakera_FilePackage * package, kakera_File * file, const char * path)
{
    
}

void kakera_SetPackageEncryptMethod(kakera_FilePackage * package, kakera_CryptMethod method)
{
    
}

void kakera_SetPackageDecryptMethod(kakera_FilePackage * package, kakera_CryptMethod method)
{
    
}
