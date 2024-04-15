#include "jsonwrapper.h"

JsonWrapper* JsonWrapper::getInstance() {
	static JsonWrapper* Instance = new JsonWrapper();
	return Instance;
}

const std::string JsonWrapper::createJsonString(std::initializer_list<std::pair<std::string, std::string>> args) {
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Document::AllocatorType allocator;

	rapidjson::Value params(rapidjson::kObjectType);
	for (const auto& arg : args) {
		rapidjson::Value key(arg.first.data(), allocator);
		rapidjson::Value value(arg.second.data(), allocator);
		params.AddMember(key, value, allocator);
	}
	document.AddMember("params", params, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	return buffer.GetString();
}

JsonWrapper::JsonWrapper() {

}