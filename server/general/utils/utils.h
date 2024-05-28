#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

class Utils {
public:
	/*
	Converts once before the first shipment to any of the clients to optimize the subsequent transfer.
	Warning: 
		(1) will cause undefined behavior when using different files 
		(2) does not read every time == server restart is required to update the target file
	
	*/
	static const std::vector<char>& ConvertFileToBytes(const std::string& path);
	static void CreateFileFromBytes(const std::string& path, const std::vector<char>& bytes);
};

#endif // !UTILS_H
