#include "logmanager.h"

std::shared_ptr<LogManager> LogManager::getInstance() {
	static std::shared_ptr<LogManager> Instance = std::make_shared<LogManager>();
	if (!Instance) {
		throw std::runtime_error("Failed to initialize log manager");
	}
	return Instance;
}

std::shared_ptr<EventLog> LogManager::getEventLog() {
	return getInstance()->m_event;
}

std::shared_ptr<ServerLog> LogManager::getServerLog() {
	std::shared_ptr<ServerLog> object = std::make_shared<ServerLog>();
	if (!object) {
		throw std::runtime_error("Failed to initialize server log file");
	}

	return object;
}

void LogManager::initEventLog() {
	m_event = std::make_shared<EventLog>();
	if (!m_event) {
		throw std::runtime_error("Failed to initialize event log file");
	}
}

void LogManager::createLog(std::shared_ptr<Log> object, const std::string& log) {
	object->write(log);
}
