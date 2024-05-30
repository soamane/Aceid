#pragma once
#ifndef HARDWARE_H
#define HARDWARE_H

#include <string>

class Hardware {
public:
	static const std::string GetHardwareId();

private:
	static const std::string GetCPUInfo();
	static const std::string GetGPUInfo();
};

#endif // !HARDWARE_H
