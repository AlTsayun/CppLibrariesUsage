#ifndef LAB3_MEMORY_REPLACE_H
#define LAB3_MEMORY_REPLACE_H

#if defined(BUILD_DLL)
# define DLL_EXP __declspec(dllexport)
#else
# if defined(BUILD_APP)
# define DLL_EXP __declspec(dllimport)
# else
# define DLL_EXP
# endif
#endif

#include <Windows.h>

DLL_EXP BOOL replaceData(DWORD pid, const char *data, size_t dataLen, const char *replacement, size_t replacementLen);

#endif //LAB3_MEMORY_REPLACE_H
