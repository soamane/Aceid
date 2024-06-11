#include "utils.h"

#include <windows.h>
#include <fstream>
#include <iostream>
#include "../logsystem/logmanager/logmanager.h"

const std::vector<char> Utils::ConvertFileToBytes(const std::string& path) {
    if (path.empty()) {
        throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open target file");
    }

    std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (bytes.empty()) {
        throw std::runtime_error("Failed to read file information");
    }

    file.close();
    return bytes;
}

void Utils::CreateFileFromBytes(const std::string& path, const std::vector<char>& bytes) {
    if (path.empty() || bytes.empty()) {
        throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
    }

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open target file");
    }

    file.write(bytes.data(), bytes.size());
    file.close();
}

void Utils::ExecuteObfuscation(const std::string& fileName) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    const std::string command = "VMProtect.exe aceid.exe " + fileName + " -pf script.vmp";

    if (!CreateProcessA(NULL, const_cast<LPSTR>(command.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi)) {
        throw std::runtime_error("Failed to create process");
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
