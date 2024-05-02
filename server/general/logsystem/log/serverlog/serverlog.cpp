#include "serverlog.h"

void ServerLog::write(const std::string& log)
{
    m_file.open(m_fileName, std::ios::app);
    if (!m_file.is_open( ))
    {
        throw std::runtime_error("failed open server log file");
    }

    m_file << getCurrentTime( ) << ' ' << log << std::endl;
    m_file.close( );
}
