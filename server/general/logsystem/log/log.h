#pragma once
#ifndef LOG_H
#define LOG_H

#include <string>

class Log {
public:

	virtual void write(const std::string& log) = 0;

	std::string getCurrentTime();

public:
	const std::string fileExtension = ".log";
};

#endif // !LOG_H
