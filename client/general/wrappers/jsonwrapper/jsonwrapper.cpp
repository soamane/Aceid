#include "jsonwrapper.h"

#include <stdexcept>

JsonWrapper* JsonWrapper::getInstance( )
{
	static JsonWrapper* Instance = new JsonWrapper( );
	return Instance;
}

const rapidjson::Document JsonWrapper::parseJsonString(const std::string& jsonString)
{
	rapidjson::Document document;
	document.Parse(jsonString.c_str( ));

	if (!document.IsObject( ))
	{
		throw std::runtime_error("failed to parse json document");
	}

	return document;
}

const rapidjson::Value& JsonWrapper::parseDocumentParams(rapidjson::Document& document)
{
	const rapidjson::Value& params = document["params"];

	if (!params.IsObject( ))
	{
		throw std::runtime_error("'params' field is not of object");
	}

	return params;
}

bool JsonWrapper::haveErrorField(const std::string& jsonString)
{
	return paramsFieldExist(jsonString, "error");
}

bool JsonWrapper::haveTokenField(const std::string& jsonString)
{
	return paramsFieldExist(jsonString, "token");
}

bool JsonWrapper::paramsFieldExist(const std::string& jsonString, const std::string& fieldName)
{
	auto document = parseJsonString(jsonString);
	auto& params = parseDocumentParams(document);

	return params.HasMember(fieldName.c_str( ));
}

const std::string JsonWrapper::createJsonString(std::initializer_list<std::pair<std::string, std::string>> additionals, std::initializer_list<std::pair<std::string, std::string>> args)
{
	rapidjson::Document document;
	document.SetObject( );

	rapidjson::Document::AllocatorType allocator;

	for (const auto& add : additionals)
	{
		rapidjson::Value key(add.first.data( ), allocator);
		rapidjson::Value value(add.second.data( ), allocator);
		document.AddMember(key, value, allocator);
	}

	rapidjson::Value params(rapidjson::kObjectType);
	for (const auto& arg : args)
	{
		rapidjson::Value key(arg.first.data( ), allocator);
		rapidjson::Value value(arg.second.data( ), allocator);
		params.AddMember(key, value, allocator);
	}
	document.AddMember("params", params, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	return buffer.GetString( );
}

const std::string JsonWrapper::parseSessionToken(const std::string& jsonString)
{
	auto document = parseJsonString(jsonString);
	auto& params = parseDocumentParams(document);

	if (!params.HasMember("token"))
	{
		throw std::runtime_error("failed to parse token field");
	}

	return params["token"].GetString( );
}

JsonWrapper::JsonWrapper( )
{

}