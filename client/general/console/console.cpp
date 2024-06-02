#include "console.h"

#include <iostream>
#include <conio.h>

#include "../../client/api/api.h"

#include "../../general/hardware/hardware.h"
#include "../../general/protect/xorstring/xorstring.h"

Console::Console() {
	SetConsoleTitleA(xorstr_(" "));
	SetConsoleSize(m_consoleWidth, m_consoleHeight);
}
Console::~Console() {
	Console::Clear();
}
const std::string Console::GetUserCredentials(AuthData& authData, API& api) {
	PrintConsoleMessage(xorstr_("username: "));
	std::cin >> authData.username;

	PrintConsoleMessage(xorstr_("password: "));
	authData.password = GetHiddenInput();

	authData.hwid = Hardware::GetHardwareId();
	if (authData.hwid.empty()) {
		throw std::runtime_error(xorstr_("Failed to get hardware id"));
	}

	authData.launchParams = api.CreateLaunchParams();
	if (authData.launchParams.empty()) {
		throw std::runtime_error(xorstr_("Failed to get launch params"));
	}

	authData.token = api.GetSessionToken();
	if (authData.token.empty()) {
		throw std::runtime_error(xorstr_("Failed to get session token"));
	}

	const std::string credentials = api.ConvertAuthDataToJson();
	if (credentials.empty()) {
		throw std::runtime_error(xorstr_("Failed to get auth data"));
	}

	Console::Clear();
	return credentials;
}

void Console::PrintConsoleMessage(const std::string& message) {
	if (message.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (console message)"));
	}

	std::cout << message;
}

void Console::Clear() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(hConsole, &csbi);

	DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD cursorPosition = { 0, 0 };

	SetConsoleCursorPosition(hConsole, cursorPosition);

	DWORD charsWritten;
	FillConsoleOutputCharacter(hConsole, ' ', consoleSize, cursorPosition, &charsWritten);

	SetConsoleCursorPosition(hConsole, cursorPosition);
}

void Console::SetConsoleSize(int width, int height) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SMALL_RECT windowSize;
	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = width - 1;
	windowSize.Bottom = height - 1;
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	COORD bufferSize;
	bufferSize.X = width;
	bufferSize.Y = height;
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	HWND hwndConsole = GetConsoleWindow();
	if (hwndConsole != NULL) {
		LONG style = GetWindowLong(hwndConsole, GWL_STYLE);
		style &= ~WS_SIZEBOX;
		style &= ~WS_MAXIMIZEBOX;
		SetWindowLong(hwndConsole, GWL_STYLE, style);

		SetWindowPos(hwndConsole, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

std::string Console::GetHiddenInput() {
	std::string input;
	const short backspaceKey = 8;

	char ch;
	while ((ch = _getch()) != 13) {
		if (ch == backspaceKey && !input.empty()) {
			input.pop_back();
			std::cout << xorstr_("\b \b");
		} else if (ch != backspaceKey) {
			input.push_back(ch);
			std::cout << '*';
		}
	}
	std::cout << std::endl;
	return input;
}
