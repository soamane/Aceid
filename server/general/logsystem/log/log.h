#pragma once
#ifndef LOG_H
#define LOG_H

#include <string>

class Log {
public:
	// Виртуальная функция записи логов для переопределения в наследниках
	virtual void WriteLog(std::string_view log) = 0;

	// Возвращает текущее время 
	std::string GetCurrentServerTime();

public:
	const std::string m_fileExtension = ".log"; // Расширение для файла лога
};

#endif // !LOG_H
