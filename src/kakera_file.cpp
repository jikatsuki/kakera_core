#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //!_MSC_VER

#include "kakera_file.h"
#include "kakera_part_implementation.h"
#include <cstdio>
#include <cstdlib>
#include "pugixml/pugixml.hpp"

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
    return kakera_CreateNewFile(size, data);
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
    return new kakera_FilePackage;
}

kakera_FilePackage * kakera_LoadFilePackage(const char* path)
{
    kakera_FilePackage* package = new kakera_FilePackage;
    package->data = fopen(path, "rb+");
    char* rawIndexSize = new char[1024];
    fread(rawIndexSize, sizeof(char), 1024, package->data);
    int indexSize = atoi(rawIndexSize);
    delete rawIndexSize;
    char* rawXMLIndex = new char[indexSize];
    fread(rawXMLIndex, sizeof(char), indexSize, package->data);
    package->index.load_string(rawXMLIndex);
    delete rawXMLIndex;
    return package;
}

void kakera_DestroyFilePackage(kakera_FilePackage * package)
{
    fclose(package->data);
    delete package;
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
