#pragma once
#ifndef FILES_CONTROLLER_H
#define FILES_CONTROLLER_H

#include <vector>
#include <string>

/*
	Класс для работы с файлами
*/
class FilesController {
public:
	// Конвертирует указанный файл с диска в массив символов 
	static const std::vector<char> ConvertFileToBytes(const std::string& path);

	// Создает файл на диске, извлекая данные из массива символов
	static void CreateFileFromBytes(const std::string& path, const std::vector<char>& bytes);
};

#endif // !FILES_CONTROLLER_H
