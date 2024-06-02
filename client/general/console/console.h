#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

#include "../../client/api/api.h"

class Console {
public:
	static const std::string GetUserCredentials(AuthData& authData, API& api);
	static void PrintConsoleMessage(const std::string& message);
	static void Clear();
};

#endif // !CONSOLE_H
