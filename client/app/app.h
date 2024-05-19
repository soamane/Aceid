#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>

class Application {
public:
	void createApplicationMutex();
	~Application();

private:
	HANDLE appHandle;
};

#endif // !APPLICATION_H
