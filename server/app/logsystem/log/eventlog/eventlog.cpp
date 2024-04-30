#include "eventlog.h"

#include <random>
#include <windows.h>

#include <boost/format.hpp>

EventLog::EventLog() {
    m_path = m_tempPath + getRandomFileName();
}

void EventLog::write(const std::string& log) {
    m_file.open(m_path, std::ios::app);
    if (!m_file.is_open()) {
        throw std::runtime_error("failed to open event log file, file path: " + m_path);
    }

    m_file << getCurrentTime() << ' ' << log << std::endl;
    m_file.close();
}

void EventLog::renameAndMove(const std::string& newFileName) {
    if (m_file.is_open()) {
        m_file.close();
    }

    std::string source = m_defaultPath + newFileName + ".log";
    DeleteFileA(source.c_str());

    const std::string newPath = source;

    std::rename(m_path.c_str(), newPath.c_str());
    m_path = newPath;
}

EventLog::~EventLog() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

std::string EventLog::getRandomFileName() {
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<> dis('a', 'z');

    std::string fileName;
    std::size_t length = 10;
    for (int i = 0; i < length; ++i) {
        fileName.push_back(static_cast<char>(dis(gen)));
    }

    return fileName + ".log";
}
