#include "eventlog.h"

#include <random>

#include <boost/format.hpp>

EventLog::EventLog() {
    this->filePath = this->tempFilePath + this->getRandomFileName();
}

void EventLog::write(const std::string& log) {
    this->file.open(this->filePath, std::ios::app);
    if (!this->file.is_open()) {
        throw std::runtime_error("failed to open event log file");
    }

    this->file << this->getCurrentTime() << ' ' << log << std::endl;
    this->file.close();
}

void EventLog::renameAndMove(const std::string& newFileName) {
    if (this->file.is_open()) {
        this->file.close();
    }

    boost::format source = boost::format("%1%%2%.log") % this->defaultFilePath % newFileName;
    const std::string newPath = source.str();

    std::rename(this->filePath.c_str(), newPath.c_str());
    this->filePath = newPath;
}

EventLog::~EventLog() {
    if (this->file.is_open()) {
        this->file.close();
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
