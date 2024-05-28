#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

class Console {
public:
	static const std::string GetUserCredentials();
	static void PrintConsoleMessage(const std::string& message);
	static void Clear();
};

#endif // !CONSOLE_H
