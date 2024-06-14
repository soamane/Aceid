#include "utils.h"

#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>

#include "../logsystem/logmanager/logmanager.h"

const std::vector<char> Utils::ConvertFileToBytes(std::string_view path) {
    if (path.empty()) {
        throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
    }

    std::ifstream file(path.data(), std::ios::binary);
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

void Utils::CreateFileFromBytes(std::string_view path, const std::vector<char>& bytes) {
    if (path.empty() || bytes.empty()) {
        throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
    }

    std::ofstream file(path.data(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open target file");
    }

    file.write(bytes.data(), bytes.size());
    file.close();
}

const std::string Utils::ExecuteObfuscation(std::string_view input, std::string_view output) {
    // Проверка на уже существующий билд под клиента
    const std::string outputPath = "builds/" + std::string(output);
    if (std::filesystem::exists(outputPath)) {
        return outputPath;
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    const std::string command = "VMProtect.exe " + std::string(input) + " " + outputPath + " -pf script.vmp";

    if (!CreateProcessA(NULL, const_cast<LPSTR>(command.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi)) {
        throw std::runtime_error("Failed to create process");
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return outputPath;
}
