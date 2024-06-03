#include "logmanager.h"

std::shared_ptr<LogManager> LogManager::GetInstance() {
	static std::shared_ptr<LogManager> Instance = std::make_shared<LogManager>();
	if (!Instance) {
		throw std::runtime_error("Failed to initialize log manager");
	}
	return Instance;
}

std::shared_ptr<EventLog> LogManager::GetEventLog() {
	return GetInstance()->m_event;
}

std::shared_ptr<ServerLog> LogManager::GetServerLog() {
	std::shared_ptr<ServerLog> object = std::make_shared<ServerLog>();
	if (!object) {
		throw std::runtime_error("Failed to initialize server log file");
	}

	return object;
}

void LogManager::InitEventLog() {
	m_event = std::make_shared<EventLog>();
	if (!m_event) {
		throw std::runtime_error("Failed to initialize event log file");
	}
}

void LogManager::CreateLog(std::shared_ptr<Log> object, const std::string& log) {
	object->WriteLog(log);
}
