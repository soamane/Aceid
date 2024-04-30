#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

class Utils
{
public:
	static const std::vector<char> convertFileToBytes(const std::string& path);
	static void createFileFromBytes(const std::string& path, const std::vector<char>& bytes);
};

#endif // !UTILS_H
