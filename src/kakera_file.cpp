#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //!_MSC_VER

#include "kakera_file.h"
#include "kakera_part_implementation.h"
#include "kakera_tools.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
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

char * kakera_GetRawFileData(kakera_File * file)
{
    return file->data;
}

int kakera_GetRawFileSize(kakera_File * file)
{
    return file->size;
}

void kakera_DestroyFile(kakera_File * file)
{
    delete file->data;
    delete file;
}

class kakera_XMLSaveAsString : public pugi::xml_writer
{
public:
    string result;
    void write(const void* data, size_t size) override
    {
        result += string(static_cast<const char*>(data), size);
    }
};

kakera_FilePackage * kakera_CreateNewFilePackage(const char* path)
{
    kakera_FilePackage* package = new kakera_FilePackage;
    package->path = path;
    pugi::xml_node xmlDeclaration = package->index.prepend_child(pugi::node_declaration);
    xmlDeclaration.append_attribute("version") = "1.0";
    xmlDeclaration.append_attribute("encoding") = "utf-8";
    pugi::xml_node filepackRoot = package->index.append_child("FilePackage");
    package->data = fopen(path, "wb+");
    kakera_XMLSaveAsString writer;
    package->index.save(writer);
    fwrite(writer.result.data(), sizeof(char), sizeof(char) * 4096, package->data);
    return package;
}

kakera_FilePackage * kakera_LoadFilePackage(const char* path)
{
    kakera_FilePackage* package = new kakera_FilePackage;
    package->path = path;
    package->data = fopen(path, "rb+");    
    char* rawXMLIndex = new char[sizeof(char) * 4096];
    fread(rawXMLIndex, sizeof(char), sizeof(char) * 4096, package->data);
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
    auto result = splitString(path, '/');
    string packagePath = "/";
    if (result.size() > 2)
    {
        for (int i = 1; i < result.size() - 1; i++)
        {
            packagePath += result[i];
            if (i != result.size() - 2)
            {
                packagePath += "/";
            }
        }
    }
    for (auto node : package->index.child("FilePackage").children())
    {
        if (strcmp(node.name(), "File") == 0 &&
            strcmp(node.attribute("path").as_string(), packagePath.c_str()) == 0 &&
            strcmp(node.attribute("name").as_string(), result[result.size() - 1].c_str()) == 0)
        {
            fseek(package->data, 4096 + node.attribute("offset").as_int(), SEEK_SET);
            char* buffer = new char[node.attribute("size").as_int()];
            fread(buffer, sizeof(char), node.attribute("size").as_int(), package->data);
            if (package->decrypt != nullptr)
            {
                return package->decrypt(kakera_CreateNewFile(node.attribute("size").as_int(), buffer));
            }
            else
            {
                return kakera_CreateNewFile(node.attribute("size").as_int(), buffer);
            }            
        }
    }
    return nullptr;
}

void kakera_AddFileToPackage(kakera_FilePackage * package, kakera_File * file, const char * path)
{
    auto result = splitString(path, '/');
    int offset = 0;
    string packagePath = "/";
    kakera_File* finalFile;
    if (package->encrypt != nullptr)
    {
        finalFile = package->encrypt(file);
    }
    else
    {
        finalFile = file;
    }
    if (result.size() > 2)
    {
        for (int i = 1; i < result.size() - 1; i++)
        {
            packagePath += result[i];
            if (i != result.size() - 2)
            {
                packagePath += "/";
            }
        }
    }
    if (!package->index.child("FilePackage").empty())
    {
        for (auto node : package->index.child("FilePackage").children())
        {
            offset += node.attribute("size").as_int();
        }
    }
    pugi::xml_node fileNode = package->index.child("FilePackage").append_child("File");
    fileNode.append_attribute("name") = result[result.size() - 1].c_str();
    fileNode.append_attribute("path") = packagePath.c_str();
    fileNode.append_attribute("offset") = offset;
    fileNode.append_attribute("size") = finalFile->size;
    kakera_XMLSaveAsString writer;
    package->index.save(writer);
    if (writer.result.size() > 4096)
    {
        return;
    }
    fseek(package->data, 0, SEEK_END);
    fwrite(finalFile->data, sizeof(char), sizeof(char) * finalFile->size, package->data);
    fseek(package->data, 0, SEEK_SET);
    fwrite(writer.result.data(), sizeof(char), sizeof(char) * 4096, package->data);
}

void kakera_SetPackageEncryptMethod(kakera_FilePackage * package, kakera_CryptMethod method)
{
    package->encrypt = method;
}

void kakera_SetPackageDecryptMethod(kakera_FilePackage * package, kakera_CryptMethod method)
{
    package->decrypt = method;
}
