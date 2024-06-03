#pragma once
#ifndef HARDWARE_H
#define HARDWARE_H

#include <string>

/*
	Класс для получения уникального идентификатора компьютера клиента
*/
class Hardware {
public:

	// Возвращает сформированные данные 
	static const std::string GetHardwareId();

private:
	// Возвращает информацию о данных процессора
	static const std::string GetCPUInfo();

	// Возвращает информацию о данных видеокарты
	static const std::string GetGPUInfo();
};

#endif // !HARDWARE_H
