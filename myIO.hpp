#pragma once
#include <iostream>
#include <string>

enum class OutputMode {
	_ERROR = 0,
	_INFO = 1,
	_SUCCESS = 2
};

void print(OutputMode mode, std::string message) {
	switch (mode) {
	case OutputMode::_SUCCESS:
		std::cout << "[+] " << message << std::endl;
		break;
	case OutputMode::_INFO:
		std::cout << "[*] " << message << std::endl;
		break;
	case OutputMode::_ERROR:
		std::cerr << "[-] " << message << std::endl;
		break;

	}
}