#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //!_MSC_VER

#include "kakera_file.h"
#include <cstdio>

using namespace std;

char * kakera_ReadFile(const char * filepath)
{
    FILE* file = fopen(filepath, "rb");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    char* result = new char[size];
    fread(result, sizeof(char), size, file);
    fclose(file);
    return result;
}
