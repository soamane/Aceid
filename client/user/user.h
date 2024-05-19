#pragma once
#ifndef USER_H
#define USER_H

#include <windows.h>

class User {
public:
	void createApplicationMutex();
	~User();
private:
	HANDLE appHandle;
};

#endif // !USER_H
