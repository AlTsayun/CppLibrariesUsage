#define BUILD_DLL

#include "memory_replace.h"
#include <Windows.h>
#include <iostream>
#include <vector>

BOOL replaceData(DWORD pid, const char *data, size_t dataLen, const char *replacement, size_t replacementLen)
{
    BOOL res = false;
    HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION, FALSE, pid);
    if(process)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        MEMORY_BASIC_INFORMATION info;
        std::vector<char> chunk;
        char* pageBaseAddress = nullptr;
        while(pageBaseAddress < si.lpMaximumApplicationAddress)
        {
            if(VirtualQueryEx(process, pageBaseAddress, &info, sizeof(info)) == sizeof(info))
            {
                pageBaseAddress = (char*)info.BaseAddress;

                if(info.State != MEM_FREE){
                    chunk.resize(info.RegionSize);
                    SIZE_T bytesRead;
                    if(ReadProcessMemory(process, pageBaseAddress, &chunk[0], info.RegionSize, &bytesRead))
                    {
                        for(size_t i = 0; i < (bytesRead - dataLen); ++i)
                        {

                            if(memcmp(data, &chunk[i], dataLen) == 0)
                            {
                                char* dataInMemory = (char*)pageBaseAddress + i;
                                SIZE_T numberOfBytesWritten = 0;
                                if (!WriteProcessMemory(process, (LPVOID) (dataInMemory), (LPVOID) replacement, replacementLen + 1, &numberOfBytesWritten)){
                                    std::cerr<< "Couldn't write to process memory at " << ((LPVOID) dataInMemory) << " error code:" << GetLastError() << std::endl;
                                } else{
                                    std::clog<< numberOfBytesWritten << " bytes written to " << ((LPVOID) dataInMemory) << std::endl;
                                    res = true;
                                }
                            }
                        }
                    } else{
                        std::cerr<< "Couldn't read process memory at " << ((LPVOID) pageBaseAddress) << " error code:" << GetLastError() << std::endl;
                    }
                }
                std::cerr <<"Skipped memory at " << ((LPVOID) pageBaseAddress) << " because it is MEM_FREE" << std::endl;
                pageBaseAddress += info.RegionSize;
            }
        }
    }
    return res;
}
