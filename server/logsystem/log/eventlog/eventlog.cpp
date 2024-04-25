#include "eventlog.h"

#include <random>

#include <boost/format.hpp>

EventLog::EventLog() {
    filePath = tempFilePath + getRandomFileName();
}

void EventLog::write(const std::string& log) {
    file.open(filePath, std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open event log file");
    }

    file << getCurrentTime() << ' ' << log << std::endl;
    file.close();
}

void EventLog::renameAndMove(const std::string& newFileName) {
    if (file.is_open()) {
        file.close();
    }

    boost::format source = boost::format("%1%%2%.log") % defaultFilePath % newFileName;
    const std::string newPath = source.str();

    std::rename(filePath.c_str(), newPath.c_str());
    filePath = newPath;
}

EventLog::~EventLog() {
    if (file.is_open()) {
        file.close();
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
