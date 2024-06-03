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

	// ѕереименовывает файл лога и перемещает в каталог авторизированных пользователей
	void RenameAndMove(const std::string& newFileName);

	~EventLog();

private:

	// √енерирует рандомную строку дл€ дальнейшего использовани€ в имени временного лога
	const std::string GenerateRandomFileName();

private:
	std::ofstream m_file;
	std::string m_path;

	const std::string m_defaultPath = "logs\\"; // Ќазвание каталога содержащего логи авторизированных сокетов
	const std::string m_tempPath = m_defaultPath + "temp\\";  // Ќазвание каталога содержащего неавторизированные сокеты
};

#endif // !EVENT_LOG_H
