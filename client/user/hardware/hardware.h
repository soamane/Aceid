#pragma once
#ifndef HARDWARE_H
#define HARDWARE_H

#include <string>

class Hardware {
public:
	static const std::string getHardwareId();

private:
	static const std::string getCPUInfo();
	static const std::string getGPUInfo();
};

#endif // !HARDWARE_H
