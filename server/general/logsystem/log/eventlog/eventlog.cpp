#include "eventlog.h"

#include <random>
#include <windows.h>

#include <boost/format.hpp>

EventLog::EventLog() {
    m_path = m_tempPath + GenerateRandomFileName();
}

void EventLog::WriteLog(std::string_view log) {
    if (log.empty()) {
        throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
    }

    m_file.open(m_path, std::ios::app);
    if (!m_file.is_open()) {
        throw std::runtime_error("failed to open event log file: " + m_path);
    }

    m_file << GetCurrentServerTime() << ' ' << log << std::endl;
    m_file.close();
}

void EventLog::RenameAndMove(std::string_view newFileName) {
    if (newFileName.empty()) {
        throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
    }

    if (m_file.is_open()) {
        m_file.close();
    }

    const std::string newPath = m_defaultPath + std::string(newFileName) + m_fileExtension;
    DeleteFileA(newPath.c_str());

    std::rename(m_path.c_str(), newPath.c_str());
    m_path = newPath;
}

EventLog::~EventLog() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

const std::string EventLog::GenerateRandomFileName() {
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<> dis('a', 'z');

    std::string fileName;
    std::size_t length = 10;
    for (int i = 0; i < length; ++i) {
        fileName.push_back(static_cast<char>(dis(gen)));
    }

    return fileName + m_fileExtension;
}
