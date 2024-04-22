#pragma once
#ifndef EVENT_LOG_H
#define EVENT_LOG_H

#include <string>
#include <fstream>

#include "../log.h"

class EventLog : public Log
{
public:
	EventLog();

	void write(const std::string& log);
	void renameAndMove(const std::string& newFileName);

	~EventLog();

private:
	std::string getRandomFileName();

private:
	std::ofstream file;
	std::string filePath;

	const std::string defaultFilePath = "logs\\";
	const std::string tempFilePath = defaultFilePath + "temp\\";
};

#endif // !EVENT_LOG_H
