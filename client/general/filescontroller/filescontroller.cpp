#include "filescontroller.h"

#include <fstream>

#include "../protect/xorstring/xorstring.h"

const std::vector<char> FilesController::ConvertFileToBytes(const std::string& path) {
	if (path.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (path)"));
	}

	std::ifstream file(path, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error(xorstr_("Failed to open target file"));
	}

	std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	if (bytes.empty()) {
		throw std::runtime_error(xorstr_("Failed to read file information"));
	}

	file.close();
	return bytes;
}

void FilesController::CreateFileFromBytes(const std::string& path, const std::vector<char>& bytes) {
	if (path.empty() || bytes.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (path/bytes)"));
	}

	std::ofstream file(path, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error(xorstr_("Failed to open target file"));
	}

	file.write(bytes.data(), bytes.size());
	file.close();
}