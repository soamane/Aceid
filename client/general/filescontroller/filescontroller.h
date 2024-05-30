#pragma once
#ifndef FILES_CONTROLLER_H
#define FILES_CONTROLLER_H

#include <vector>
#include <string>

class FilesController {
public:
	static const std::vector<char> ConvertFileToBytes(const std::string& path);
	static void CreateFileFromBytes(const std::string& path, const std::vector<char>& bytes);
};

#endif // !FILES_CONTROLLER_H
