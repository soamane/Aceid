#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

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
	static void PrintConsoleMessage(std::string_view message);

	// Очищает консоль
	static void Clear();

	// Устанавливает визуальную загрузку в консоли
	static void SetLoadingMessage();
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

	/* Проверяет наличие русский символов в строке */
	bool hasRussianCharacters(std::string_view str);

private:
	const int m_consoleWidth = 50; // Ширина окна консоли
	const int m_consoleHeight = 15; // Высота окна консоли
};

#endif // !CONSOLE_H
