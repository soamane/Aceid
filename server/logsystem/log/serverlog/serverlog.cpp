#include "serverlog.h"

void ServerLog::write(const std::string& log) {
    file.open(fileName, std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("failed open server log file");
    }

    file << getCurrentTime() << ' ' << log << std::endl;
    file.close();
}
