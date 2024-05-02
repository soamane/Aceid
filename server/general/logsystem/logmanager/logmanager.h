#pragma once
#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <memory>

#include "../log/log.h"
#include "../log/eventlog/eventlog.h"
#include "../log/serverlog/serverlog.h"

/*

	Logger class manager
	(Singleton pattern)
*/

class LogManager
{
public:
	// Obitain a pointer to an object of a class
	static std::shared_ptr<LogManager> getInstance( );

	// Obitain a pointer to an object of the EventLog class
	static std::shared_ptr<EventLog> getEventLog( );

	// Obitain a pointer to an object of the ServerLog class
	static std::shared_ptr<ServerLog> getServerLog( );

	// Initializes the event log file
	void initEventLog( );

	// Function to create a log for virtual objects
	void createLog(std::shared_ptr<Log> object, const std::string& log);

private:
	std::shared_ptr<EventLog> m_event;
};

#define CREATE_EVENT_LOG(log) LogManager::getInstance()->createLog(LogManager::getInstance()->getEventLog(), log);
#define CREATE_SERVER_LOG(log) LogManager::getInstance()->createLog(LogManager::getInstance()->getServerLog(), log);

#endif // !LOG_MANAGER_H
