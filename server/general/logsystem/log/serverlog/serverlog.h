#pragma once
#ifndef SERVER_LOG_H
#define SERVER_LOG_H

#include <string>
#include <fstream>

#include "../log.h"

class ServerLog : public Log {
public:
    void WriteLog(std::string_view log) override;

public:
    std::ofstream m_file;
    std::string m_fileName = "server.log"; // Имя файла лога
};

#endif // !SERVER_LOG_H
