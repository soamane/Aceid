#pragma once
#ifndef SERVER_LOG_H
#define SERVER_LOG_H

#include <string>
#include <fstream>

#include "../log.h"

class ServerLog : public Log
{
public:
    void write(const std::string& log) override;

public:
    std::ofstream file;
    std::string fileName = "server.log";
};

#endif // !SERVER_LOG_H
