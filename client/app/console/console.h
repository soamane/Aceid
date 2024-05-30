#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

#include "../../client/api/api.h"

class Console {
public:
	static const std::pair<std::string, AuthData&> GetUserData();
	static void PrintConsoleMessage(const std::string& message);
	static void Clear();
};

#endif // !CONSOLE_H
