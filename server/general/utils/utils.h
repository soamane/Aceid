#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

class Utils {
public:
	/*
		Конвертирует указанный файл в массив символов
	*/
	static const std::vector<char> ConvertFileToBytes(std::string_view path);

	// Создает на диске файл из поступаемого массива байт по указанному пути
	static void CreateFileFromBytes(std::string_view path, const std::vector<char>& bytes);

	/*
		Запускает обфускацию VMProtect для input файла и сохраняет его в output файл
		Примечание: возвращает относительный путь к зашифрованному файлу
	*/ 	
	static const std::string ExecuteObfuscation(std::string_view input, std::string_view output);
};

#endif // !UTILS_H
