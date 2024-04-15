#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <utility>
#include <initializer_list>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class JsonWrapper
{
public:
	static JsonWrapper* getInstance();

	const std::string createJsonString(std::initializer_list<std::pair<std::string, std::string>> args);

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H
