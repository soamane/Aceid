#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <rapidjson/document.h>

/*
	��������� ������ ������� ��� ��������������
*/
struct AuthData {
	std::string username;
	std::string password; 
	std::string hwid; // ���������� ID ���������� 

	std::string token; // ����� ������� ������ � ��������
	std::string member_id; 
	std::string profile_group; 
};

/*
	�����-������ ���������� RapidJSON
	������������, ������������ � ������ ����������� ������
	����������: ��������������� ��� ��������� ������ ����������� � �������� ������ �� web-������� � ������ �������
*/
class JsonWrapper {
public:
	static JsonWrapper* GetInstance();

	// ������������ ����������� ������ � �������� JSON ������� 
	const rapidjson::Document ConvertStringToJson(const std::string& jsonString) const;

	// ���������� ���� "params" � �������������� ��������� JSON
	const rapidjson::Value& ParseDocumentParams(rapidjson::Document& document) const;

	// ��������� ������� ���� "error" � ����� JSON ������
	const bool IsErrorField(const std::string& jsonString) const;

	// ��������� ������� ���� "id" � ����� JSON ������
	const bool IsMemberIdField(const std::string& jsonString) const;

	// ��������� ������������� ���� � ��������� ������ 
	const bool ParamsFieldExist(const std::string& jsonString, const std::string& fieldName) const;

	// ������ ������ �� ���������� ���� � ���������� "params"
	const std::string ParseParamsField(const std::string& jsonString, const std::string& fieldName) const;

	/*
		������ ������� ������ ������������ � ��������� AuthData
		����������: �� ���� ��������� ������ JSON ������� �� ������� (������� ������)
	*/
	const AuthData ParseUserData(const std::string& jsonString) const;

	/*
		������������ ��������� ������� � ������ JSON �������
		������������ ���������� ���������� (����������)
	*/
	const std::string CreateJsonString(std::initializer_list<std::pair<std::string, std::string>> args, std::initializer_list<std::pair<std::string, std::string>> params) const;

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H

