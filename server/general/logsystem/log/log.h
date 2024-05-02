#pragma once
#ifndef LOG_H
#define LOG_H

#include <string>

/*

	Base log class

*/

class Log
{
public:

	// Virtual function for writing data to the log
	virtual void write(const std::string& log) = 0;

	// Returns a string with the current date and time in a specific format
	std::string getCurrentTime( );
};

#endif // !LOG_H
