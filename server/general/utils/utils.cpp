#include "utils.h"

#include <fstream>

#include "../logsystem/logmanager/logmanager.h"

const std::vector<char> Utils::convertFileToBytes(const std::string& path)
{
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open( ))
	{
		CREATE_EVENT_LOG("Failed to open target file")
			return std::vector<char>( );
	}

	std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>( ));
	file.close( );

	CREATE_EVENT_LOG("File converted to bytes array")
		return bytes;
}

void Utils::createFileFromBytes(const std::string& path, const std::vector<char>& bytes)
{
	std::ofstream file(path, std::ios::binary);
	if (!file.is_open( ))
	{
		CREATE_EVENT_LOG("Failed to open target file")
			return;
	}

	file.write(bytes.data( ), bytes.size( ));
	file.close( );

	CREATE_EVENT_LOG("File converted from bytes array")
}
