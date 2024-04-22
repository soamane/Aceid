#include "logmanager.h"

std::shared_ptr<LogManager> LogManager::getInstance() {
	static std::shared_ptr<LogManager> Instance = std::make_shared<LogManager>();
	return Instance;
}

std::shared_ptr<EventLog> LogManager::getEventLog() {
	std::shared_ptr<EventLog> object = getInstance()->eventLog;
	if (object == nullptr) {
		throw std::runtime_error("event log is not initialized");
	}

	return object;
}

std::shared_ptr<ServerLog> LogManager::getServerLog() {
	std::shared_ptr<ServerLog> object = std::make_shared<ServerLog>();
	if (object == nullptr) {
		throw std::runtime_error("server log is not initialized");
	}

	return object;
}

void LogManager::initEventLog() {
	this->eventLog = std::make_shared<EventLog>();
}

void LogManager::createLog(std::shared_ptr<Log> object, const std::string& log) {
	object->write(log);
}
