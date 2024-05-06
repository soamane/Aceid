#pragma once
#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <memory>

#include "../log/log.h"
#include "../log/eventlog/eventlog.h"
#include "../log/serverlog/serverlog.h"

class LogManager {
public:
	static std::shared_ptr<LogManager> getInstance();

	static std::shared_ptr<EventLog> getEventLog();

	static std::shared_ptr<ServerLog> getServerLog();

	void initEventLog();

	void createLog(std::shared_ptr<Log> object, const std::string& log);

private:
	std::shared_ptr<EventLog> m_event;
};

#define CREATE_EVENT_LOG(log) LogManager::getInstance()->createLog(LogManager::getInstance()->getEventLog(), log);
#define CREATE_SERVER_LOG(log) LogManager::getInstance()->createLog(LogManager::getInstance()->getServerLog(), log);

#endif // !LOG_MANAGER_H
