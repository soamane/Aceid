#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <rapidjson/document.h>

/*

	Client data structure for authorization

*/
struct AuthData {
	std::string username;
	std::string password;
	std::string hwid; // computer hardware id 

	std::string token; // session token from forum (site)
	std::string member_id; // client ID from the site
};

/*

	Wrapper for the rapidjson library for convenient document handling.
	(Singleton pattern)

*/
class JsonWrapper {
public:

	// Obtaining a pointer to an object of a class.
	static JsonWrapper* getInstance();

	// Converts a string to a JSON document
	const rapidjson::Document parseJsonString(const std::string& jsonString);

	// Parses the 'params' field from the JSON document
	const rapidjson::Value& parseDocumentParams(rapidjson::Document& document);

	// Checks for the presence of the 'error' field in the JSON document
	bool haveErrorField(const std::string& jsonString);

	// Checks for the presence of the 'id' field in the JSON document
	bool haveMemberIdField(const std::string& jsonString);

	// General function for checking the presence of the nth field
	bool paramsFieldExist(const std::string& jsonString, const std::string& fieldName);

	// Parses the 'id' field from the JSON document
	const std::string parseMemberId(const std::string& jsonString);

	// Parses the entire AuthData structure from the JSON document
	const AuthData parseUserData(const std::string& jsonString);

	// Converts incoming arguments to a JSON string
	const std::string createJsonString(std::initializer_list<std::pair<std::string, std::string>> args, std::initializer_list<std::pair<std::string, std::string>> params);

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H

