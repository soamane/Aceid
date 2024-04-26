#pragma once
#ifndef EVENT_LOG_H
#define EVENT_LOG_H

#include <string>
#include <fstream>

#include "../log.h"

/*

	Class for working with event logs (ñlient events, verification statuses, and more)

*/

class EventLog : public Log
{
public:
	EventLog();

	// Log writing function
	void write(const std::string& log);

	// Renames the log file to the specified name 
	// and moves it to the main directory of authorized logs
	void renameAndMove(const std::string& newFileName);

	~EventLog();

private:

	// Generates a random name for the temp log file
	std::string getRandomFileName();

private:
	std::ofstream m_file;
	std::string m_path;

	const std::string m_defaultPath = "logs\\"; // main log directory
	const std::string m_tempPath = m_defaultPath + "temp\\"; // temp log directory
};

#endif // !EVENT_LOG_H
