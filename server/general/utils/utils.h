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
	static const std::vector<char> ConvertFileToBytes(const std::string& path);

	// Создает на диске файл из поступаемого массива байт по указанному пути
	static void CreateFileFromBytes(const std::string& path, const std::vector<char>& bytes);

	/*
		Запускает обфускацию VMProtect для input файла и сохраняет его в output файл
		Примечание: возвращает относительный путь к зашифрованному файлу
	*/ 	
	static const std::string ExecuteObfuscation(const std::string& input, const std::string& output);
};

#endif // !UTILS_H
