#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

#include "../../client/api/api.h"

/*
	Класс для визуализирования в консольном окне
*/
class Console {
public:
	// Устанавливает имя окна и параметры консоли
	Console();

	// Вызывает метод очистки консоли
	~Console();

	// Возвращает полученные данные от клиента в формате JSON строки и заполняет структуру данных AuthData
	const std::string GetUserCredentials(AuthData& authData, API& api);

	// Выводит указанную строку в консоль
	static void PrintConsoleMessage(const std::string& message);

	// Очищает консоль
	static void Clear();

private:
	/*
		Устанавливает настройки консоли

		Действия: 
		1. Устанавливает размер консоли
		2. Устанавливает размер буфера
		3. Устанавливает настройки консоли 
	*/
	void SetConsoleProperties();

	/*
		Скрывает поступаемые символы на знак '*'
		Примечание: используется для защищенного ввода пароля
	*/
	const std::string GetHiddenInput();

private:
	const int m_consoleWidth = 50; // Ширина окна консоли
	const int m_consoleHeight = 15; // Высота окна консоли
};

#endif // !CONSOLE_H
