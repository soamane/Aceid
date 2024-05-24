#include "serverlog.h"

void ServerLog::write(const std::string& log) {
    if (log.empty()) {
        throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
    }

    m_file.open(m_fileName, std::ios::app);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed open server log file");
    }

    m_file << getCurrentTime() << ' ' << log << std::endl;
    m_file.close();
}
