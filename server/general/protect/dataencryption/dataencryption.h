#pragma once
#ifndef DATA_ENCRYPTION_H
#define DATA_ENCRYPTION_H

#include <string>
#include <vector>

/*
	Класс шифрования данных
*/
class DataEncryption {
public:
	// Шифрование строки обычным base64
	static const std::string EncryptBase64(std::string_view source);

	// Дешифрование строки обычным base64
	static const std::string DecryptBase64(std::string_view source);

	/*
		Шифрование усложнённой версией base64
		Примечание: Используется шифрование каждого отдельного символа строки с помощью base64
					Полученные символы '=' заменяются на значение соли шифрования
					
	*/ 
	static const std::string EncryptMultiBase64(std::string_view source);

	// Дешифрование усложнённой версией base64
	static const std::string DecryptMultiBase64(std::string_view source);

	/*
		Шифрование кастомным методом
		Примечание: В основу шифрования заложен алгоритм base64

		Действия:
		1. От каждого ASCII кода символа исходной строки отнимается ASCII код символа соли шифрования 
		   (каждый бит символа соли ксориться (xor, ^) с битом размера этой соли
		2. Полученный набор ASCII символов переводится в строковое представление
		3. Полученная строка обрабатывается методом усложнённой версии base64
	*/
	static const std::string EncryptCustomMethod(std::string_view source);

	// Дешифрование кастомного метода
	static const std::string DecryptCustomMethod(std::string_view source);

	/*
		Шифрование массива символов
		Примечание: заложена логика от шифрования кастомным методом
	*/
	static const std::vector<char> EncryptBuffer(const std::vector<char>& source);

	// Дешифрование массива символов
	static const std::vector<char> DecryptBuffer(const std::vector<char>& source);

private:
	// Функция генерации целого числа из ASCII значений соли шифрования
	static const int GenerateKeyCode(const std::vector<int>& keyData);

private:
	static inline const std::string key = "aceid"; // Соль шифрования
};

#endif // !DATA_ENCRYPTION_H
