#include "kakera_file.h"
#include "kakera_part_implementation.h"
#include "kakera_tools.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

kakera_File * kakera_ReadFile(const char * filepath)
{
    FILE* file = fopen(filepath, "rb");
    kakera_private::CheckNullPointer(file);
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

const char * const kakera_GetRawFileData(kakera_File * file)
{
    kakera_private::CheckNullPointer(file);
    return file->data;
}

int kakera_GetRawFileSize(kakera_File * file)
{
    kakera_private::CheckNullPointer(file);
    return file->size;
}

void kakera_DestroyFile(kakera_File ** file)
{
    kakera_private::CheckNullPointer(*file);
    delete[] (*file)->data;
    delete *file;
    *file = nullptr;
}
