#pragma once
#ifndef SERVER_LOG_H
#define SERVER_LOG_H

#include <string>
#include <fstream>

#include "../log.h"

/*

    Class for working with server logs (errors, exceptions and more)

*/

class ServerLog : public Log {
public:
    // Log writing function
    void write(const std::string& log) override;

public:
    std::ofstream m_file;
    std::string m_fileName = "server.log";
};

#endif // !SERVER_LOG_H
