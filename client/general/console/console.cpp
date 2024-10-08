﻿#include "console.h"
#include "../../general/hardware/hardware.h"
#include "../../general/protect/xorstring/xorstring.h"

#include <thread>
#include <conio.h>
#include <iostream>

Console::Console() {
	SetConsoleTitleA(xorstr_(" "));
	SetConsoleProperties();
}
Console::~Console() {
	Console::Clear();
}
const std::string Console::GetUserCredentials(AuthData& authData, API& api) {
	PrintConsoleMessage(xorstr_("username: "));
	std::cin >> authData.username;

	PrintConsoleMessage(xorstr_("password: "));
	authData.password = GetHiddenInput();

	if (hasRussianCharacters(authData.username) || hasRussianCharacters(authData.password)) {
		throw std::runtime_error(xorstr_("Russsian characters in credentials"));
	}

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

void Console::PrintConsoleMessage(std::string_view message) {
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

void Console::SetLoadingMessage() {
	PrintConsoleMessage(xorstr_("\n\n\n\n\t\t     LOADING\n"));
	for (int i = 0; i < 15; ++i) {
		PrintConsoleMessage(xorstr_("\r\t\t["));
		for (int j = 0; j <= i; ++j) {
			std::cout << char(219);
		}
		for (int k = 0; k < 14 - i; ++k) {
			PrintConsoleMessage(xorstr_(" "));
		}
		PrintConsoleMessage(xorstr_("]"));
		std::this_thread::sleep_for(std::chrono::milliseconds(1750));
	}
	PrintConsoleMessage(xorstr_("\nPlease, wait..."));
}

void Console::SetConsoleProperties() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SMALL_RECT windowSize;
	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = m_consoleWidth - 1;
	windowSize.Bottom = m_consoleHeight - 1;
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	HWND hwndConsole = GetConsoleWindow();
	if (!hwndConsole) {
		throw std::runtime_error(xorstr_("Failed to set console window size"));
	}

	LONG style = GetWindowLong(hwndConsole, GWL_STYLE);
	style &= ~WS_SIZEBOX;
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(hwndConsole, GWL_STYLE, style);

	SetWindowPos(hwndConsole, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	COORD bufferSize;
	bufferSize.X = m_consoleWidth;
	bufferSize.Y = m_consoleHeight;
	SetConsoleScreenBufferSize(hConsole, bufferSize);
}

const std::string Console::GetHiddenInput() {
	std::string result;
	const short backspaceKey = 8;

	char symb;
	while ((symb = _getch()) != 13) {
		if (symb != backspaceKey) {
			result.push_back(symb);
			PrintConsoleMessage(xorstr_("*"));
		}

		if (symb == backspaceKey && !result.empty()) {
			result.pop_back();
			PrintConsoleMessage(xorstr_("\b \b"));
		}
	}

	return result;
}

bool Console::hasRussianCharacters(std::string_view str) {
	for (size_t i = 0; i < str.length(); ) {
		unsigned char c = str[i];
		// Check for Russian characters in UTF-8
		if ((c >= 0xD0 && c <= 0xD1) && (i + 1 < str.length())) {
			unsigned char next = str[i + 1];
			if ((c == 0xD0 && (next >= 0x90 && next <= 0xBF)) ||
				(c == 0xD1 && (next >= 0x80 && next <= 0x8F))) {
				return true;
			}
			i += 2;
		}
		// Check for Russian characters in Windows-1251
		else if ((c >= 0xC0 && c <= 0xFF) || c == 0xA8 || c == 0xB8) {
			return true;
		}
		else {
			++i;
		}
	}
	return false;
}