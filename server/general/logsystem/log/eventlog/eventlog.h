#pragma once
#ifndef EVENT_LOG_H
#define EVENT_LOG_H

#include <string>
#include <fstream>

#include "../log.h"

class EventLog : public Log {
public:
	EventLog();

	void WriteLog(const std::string& log);
	void RenameAndMove(const std::string& newFileName);

	~EventLog();

private:
	const std::string GenerateRandomFileName();

private:
	std::ofstream m_file;
	std::string m_path;

	const std::string m_defaultPath = "logs\\"; 
	const std::string m_tempPath = m_defaultPath + "temp\\"; 
};

#endif // !EVENT_LOG_H
