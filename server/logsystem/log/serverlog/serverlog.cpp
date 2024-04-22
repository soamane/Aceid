#include "serverlog.h"

void ServerLog::write(const std::string& log) {
    this->file.open(this->fileName, std::ios::app);
    if (!this->file.is_open()) {
        throw std::runtime_error("failed open server log file");
    }

    this->file << this->getCurrentTime() << ' ' << log << std::endl;
    this->file.close();
}
