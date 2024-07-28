#pragma once
#ifndef SERVER_LOG_H
#define SERVER_LOG_H

#include "../log.h"

class ServerLog : public Log {
public:
    void WriteLog(std::string_view log) override;

private:
    std::ofstream m_file;
    const std::string m_fileName = "server.log"; // Имя файла лога
};

#endif // SERVER_LOG_H
