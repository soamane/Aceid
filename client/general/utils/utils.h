#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

class Utils {
public:
	static const std::vector<char> ConvertFileToBytes(const std::string& path);
	static void CreateFileFromBytes(const std::string& path, const std::vector<char>& bytes);
};

#endif // !UTILS_H
