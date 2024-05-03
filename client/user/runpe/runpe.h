#pragma once
#ifndef RUNPE_HPP
#define RUNPE_HPP

#include <vector>
#include <windows.h>

class RunPE {
public:
	static void RunExecutable(std::vector<char>& Image, LPCSTR CmdLine);

private:
	static PVOID RpGetProcAddress(const char* Mod, const char* Proc);
};

#endif // !RUNPE_HPP