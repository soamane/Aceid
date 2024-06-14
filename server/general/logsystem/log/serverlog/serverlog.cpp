#include "serverlog.h"

void ServerLog::WriteLog(std::string_view log) {
    if (log.empty()) {
        throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
    }

    m_file.open(m_fileName, std::ios::app);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed open server log file");
    }

    m_file << GetCurrentServerTime() << ' ' << log << std::endl;
    m_file.close();
}
