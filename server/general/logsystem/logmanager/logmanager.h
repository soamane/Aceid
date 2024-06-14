#pragma once
#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <memory>

#include "../log/log.h"
#include "../log/eventlog/eventlog.h"
#include "../log/serverlog/serverlog.h"

class LogManager {
public:
	static std::shared_ptr<LogManager> GetInstance();
	static std::shared_ptr<EventLog> GetEventLog();
	static std::shared_ptr<ServerLog> GetServerLog();

	void InitEventLog();
	void CreateLog(std::shared_ptr<Log> object, std::string_view log);

private:
	std::shared_ptr<EventLog> m_event;
};

#define CREATE_EVENT_LOG(log) LogManager::GetInstance()->CreateLog(LogManager::GetInstance()->GetEventLog(), log);
#define CREATE_SERVER_LOG(log) LogManager::GetInstance()->CreateLog(LogManager::GetInstance()->GetServerLog(), log);

#endif // !LOG_MANAGER_H
