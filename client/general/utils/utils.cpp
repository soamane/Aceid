#include "utils.h"

#include <fstream>

const std::vector<char> Utils::convertFileToBytes(const std::string& path) {
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open()) {
		return std::vector<char>();
	}

	std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return bytes;
}

void Utils::createFileFromBytes(const std::string& path, const std::vector<char>& bytes) {
	std::ofstream file(path, std::ios::binary);
	if (!file.is_open()) {
		return;
	}

	file.write(bytes.data(), bytes.size());
	file.close();
}
