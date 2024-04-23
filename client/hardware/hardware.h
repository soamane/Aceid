#pragma once
#ifndef HARDWARE_H
#define HARDWARE_H

#include <string>

class Hardware
{
public:
	static std::string getHardwareId();

private:
	static std::string getCPUInfo();
	static std::string getGPUInfo();
};

#endif // !HARDWARE_H
