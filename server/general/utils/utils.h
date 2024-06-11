#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

class Utils {
public:
	/*
	Конвертирует файл один раз перед первой отправкой любому из клиентов, чтобы оптимизировать последующую отправку.
	Примечание: Приводит к неопределенному поведению при использовании разных файлов
				Не выполняется чтение каждый раз, т.е для обновления целевого файла требуется перезагрузка сервера
	
	*/
	static const std::vector<char> ConvertFileToBytes(const std::string& path);

	// Создает на диске файл из поступаемого массива байт по указанному пути
	static void CreateFileFromBytes(const std::string& path, const std::vector<char>& bytes);

	static void ExecuteObfuscation(const std::string& fileName);
};

#endif // !UTILS_H
