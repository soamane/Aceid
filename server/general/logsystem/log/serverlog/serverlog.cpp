#include "serverlog.h"

#include <boost/format.hpp>

void ServerLog::WriteLog(std::string_view log) {
    m_file.open(m_fileName, std::ios::app);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to open server log file: " + m_fileName);
    }

    m_file << GetCurrentServerTime() << ' ' << log << std::endl;
    m_file.close();
}
