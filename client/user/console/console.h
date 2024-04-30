#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

class Console
{
public:
	static const std::string getUserCredentials();
	static void showConsoleMessage(const std::string& message);
};

#endif // !CONSOLE_H
