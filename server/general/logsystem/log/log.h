#pragma once
#ifndef LOG_H
#define LOG_H

#include <string>

class Log {
public:

	virtual void write(const std::string& log) = 0;

	std::string getCurrentTime();
};

#endif // !LOG_H
