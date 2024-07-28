#include "eventlog.h"

#include <random>
#include <windows.h>
#include <boost/format.hpp>

EventLog::EventLog() : m_path(m_tempPath + GenerateRandomFileName()) {}

void EventLog::WriteLog(std::string_view log) {
    std::ofstream file(m_path, std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open event log file: " + m_path);
    }

    file << GetCurrentServerTime() << ' ' << log << std::endl;
}

void EventLog::RenameAndMove(std::string_view newFileName) {
    if (m_file.is_open()) {
        m_file.close();
    }

    std::string newPath = m_defaultPath + std::string(newFileName) + m_fileExtension;
    if (!DeleteFileA(newPath.c_str())) {
        throw std::runtime_error("Failed to delete existing file: " + newPath);
    }

    if (std::rename(m_path.c_str(), newPath.c_str()) != 0) {
        throw std::runtime_error("Failed to rename and move file from " + m_path + " to " + newPath);
    }
    m_path = newPath;
}

EventLog::~EventLog() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

const std::string EventLog::GenerateRandomFileName() const {
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<> dis('a', 'z');

    std::string fileName;
    std::size_t length = 10;
    for (std::size_t i = 0; i < length; ++i) {
        fileName.push_back(static_cast<char>(dis(gen)));
    }

    return fileName + m_fileExtension;
}

