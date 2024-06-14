#pragma once
#ifndef EVENT_LOG_H
#define EVENT_LOG_H

#include <string>
#include <fstream>

#include "../log.h"

class EventLog : public Log {
public:
	EventLog();

	void WriteLog(std::string_view log);

	// Переименовывает файл лога и перемещает в каталог авторизированных пользователей
	void RenameAndMove(std::string_view newFileName);

	~EventLog();

private:

	// Генерирует рандомную строку для дальнейшего использования в имени временного лога
	const std::string GenerateRandomFileName();

private:
	std::ofstream m_file;
	std::string m_path;

	const std::string m_defaultPath = "logs\\"; // Название каталога содержащего логи авторизированных сокетов
	const std::string m_tempPath = m_defaultPath + "temp\\";  // Название каталога содержащего неавторизированные сокеты
};

#endif // !EVENT_LOG_H
