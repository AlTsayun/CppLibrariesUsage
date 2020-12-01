
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include "memory_replace.h"

#define FILE_WITH_LINES_PATH "..\\resources\\lines.txt"
#define DYNAMIC_LIBRARY_PATH "memory_replace_dynamic_lib.dll"
#define LINE_MAX_LENGTH 256

typedef BOOL (/*__stdcall */*f_replaceData)(DWORD pid, const char *data, size_t dataLen, const char *replacement, size_t replacementLen);

char data [LINE_MAX_LENGTH];
char replacement[LINE_MAX_LENGTH];

BOOL replaceDataStatic(DWORD pid, const char *data, size_t dataLen, const char *replacement, size_t replacementLen);

BOOL replaceDataDynamic(DWORD pid, const char *data, size_t dataLen, const char *replacement, size_t replacementLen, const char* libraryPath);

BOOL readLinesFromFile(std::vector<char*> dest, const char* path){
    std::fstream textFile;
    BOOL res = false;
    std::filesystem::path ds = std::filesystem::current_path();
    textFile.open(path, std::ios::in);
    if(textFile.is_open()){
        std::string tmp;
        for(int i = 0;i < dest.size() && std::getline(textFile, tmp); ++i){
            strcpy_s(dest[i], LINE_MAX_LENGTH, tmp.c_str());
            res = true;
        }
    }
    return res;
}

int main() {
    readLinesFromFile({data, replacement}, FILE_WITH_LINES_PATH);

    std::cout
    << "Initial string: \"" << data << "\"" << std::endl
    << "Replacement string: \"" << replacement << "\"" << std::endl
    << "Select type of dll injection:" <<std::endl
    << "0. Static" << std::endl
    << "1. Dynamic" << std::endl
    << "2. Remote thread" << std::endl;
    char answer;
    std::cin >> answer;

    switch(answer){
        case '0':
            replaceDataStatic(GetCurrentProcessId(), data, strlen(data), replacement, strlen(data));
            break;
        case '1':
            replaceDataDynamic(GetCurrentProcessId(), data, strlen(data), replacement, strlen(data), DYNAMIC_LIBRARY_PATH);
            break;
        case '2':
            break;
        default:
            break;
    }
    std::cout
            << "Modified string: \"" << data << "\"" << std::endl;
    return 0;
}

BOOL replaceDataDynamic(DWORD pid, const char *data, size_t dataLen, const char *replacement, size_t replacementLen, const char* libraryPath){
    HINSTANCE hGetProcIDDLL = LoadLibrary(libraryPath);
    const char* funcName = "?replaceData@@YAHKPBDI0I@Z";
    if (!hGetProcIDDLL) {
        std::cerr << "Could not load the dynamic library \"" << libraryPath << "\"" << std::endl;

        return FALSE;
    }

    auto pFunction = (f_replaceData)GetProcAddress(hGetProcIDDLL, funcName);
    if (!pFunction) {
        std::cerr << "Could not locate the function \"" << funcName << "\"" << std::endl;

        return FALSE;
    }

    pFunction(pid, data, dataLen, replacement, replacementLen);

    return TRUE;
}

BOOL replaceDataStatic(DWORD pid, const char *data, size_t dataLen, const char *replacement, size_t replacementLen){
    return replaceData(pid, data, dataLen, replacement, replacementLen);
}
