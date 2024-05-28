#include "api.h"

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/logsystem/logmanager/logmanager.h"

API::API(const std::string& jsonString) {
    if (jsonString.empty()) {
        CREATE_EVENT_LOG("Function call error: empty argument [" + std::string(__func__) + "]");
        return;
    }

    GetUserData(jsonString); // get main info of user (username, password etc.)
    GetMemberId(); // fills member id into AuthData for perform user hwid request after (based on parse data by getUserData method)
    GetProfileGroupId(); // fills group id into AuthData for definition access to the cheats (based on parse data by getUserData method)
}

const AuthStatus API::GetAuthStatus() const {
    if (!CheckUserAuthentication()) {
        CREATE_EVENT_LOG("Invalid credentials");
        return AuthStatus::AUTH_INVALID_CREDENTIALS;
    }

    if (!CheckUserHwid()) {
        CREATE_EVENT_LOG("Invalid hwid");
        return AuthStatus::AUTH_INVALID_HWID;
    }

    if (!CheckUserLicense() && m_authData.profile_group.empty()) {
        CREATE_EVENT_LOG("No license");
        return AuthStatus::AUTH_INVALID_LICENSE;
    }

    if (!CheckUserToken()) {
        CREATE_EVENT_LOG("Invalid token");
        return AuthStatus::AUTH_INVALID_TOKEN;
    }

    return AuthStatus::AUTH_SUCCESS;
}

void API::GetUserData(const std::string& jsonString) {
    if (jsonString.empty()) {
        CREATE_EVENT_LOG("Function call error: empty argument [" + std::string(__func__) + "]");
        return;
    }

    m_authData = JsonWrapper::GetInstance()->ParseUserData(jsonString);
}

void API::GetProfileGroupId() {
    CREATE_EVENT_LOG("Request to get profile group");

    const std::string jsonString = JsonWrapper::GetInstance()->CreateJsonString(
        { {"action", "groups"}, {"type", "get"} },
        { {"username", m_authData.username}, {"password", m_authData.password}, {"hwid", m_authData.hwid} }
    );

    auto requestResult = PerformApiRequest(jsonString);
    if (!requestResult) {
        CREATE_EVENT_LOG("Failed to get 'groups' field value");
        return;
    }

    m_authData.profile_group = JsonWrapper::GetInstance()->ParseParamsField(*requestResult, "groups");
}

void API::GetMemberId() {
    CREATE_EVENT_LOG("Request to get member id");

    const std::string jsonString = JsonWrapper::GetInstance()->CreateJsonString(
        { {"action", "auth"} },
        { {"username", m_authData.username}, {"password", m_authData.password} }
    );

    auto requestResult = PerformApiRequest(jsonString);
    if (!requestResult) {
        CREATE_EVENT_LOG("Failed to get 'id' field value");
        return;
    }

    m_authData.member_id = JsonWrapper::GetInstance()->ParseParamsField(*requestResult, "id");
}

const bool API::CheckUserAuthentication() const {
    CREATE_EVENT_LOG("Request to verify account availability");

    const std::string jsonString = JsonWrapper::GetInstance()->CreateJsonString(
        { {"action", "auth"} },
        { {"username", m_authData.username}, {"password", m_authData.password} }
    );

    return PerformApiRequest(jsonString).has_value();
}

const bool API::CheckUserHwid() const {
    CREATE_EVENT_LOG("Request to verify hardware id");

    const std::string jsonString = JsonWrapper::GetInstance()->CreateJsonString(
        { {"action", "hwid"}, {"type", "update"} },
        { {"member_id", m_authData.member_id}, {"hwid", m_authData.hwid} }
    );

    return PerformApiRequest(jsonString).has_value();
}

const bool API::CheckUserLicense() const {
    CREATE_EVENT_LOG("Request to verify the availability license");

    const std::string jsonString = JsonWrapper::GetInstance()->CreateJsonString(
        { {"action", "license"}, {"type", "get"} },
        { {"username", m_authData.username}, {"password", m_authData.password} }
    );

    return PerformApiRequest(jsonString).has_value();
}

const bool API::CheckUserToken() const {
    CREATE_EVENT_LOG("Request to verify session token");

    const std::string jsonString = JsonWrapper::GetInstance()->CreateJsonString(
        { {"action", "session"}, {"type", "validate"} },
        { {"username", m_authData.username}, {"hwid", m_authData.hwid}, {"token", m_authData.token} }
    );

    return PerformApiRequest(jsonString).has_value();
}

std::optional<std::string> API::PerformApiRequest(const std::string& jsonString) const {
    if (jsonString.empty()) {
        CREATE_EVENT_LOG("Function call error: empty argument [" + std::string(__func__) + "]");
        return std::nullopt;
    }

    CREATE_EVENT_LOG("Detailed information on request: ");

    const std::string encryptedJson = DataEncryption::EncryptBase64(jsonString);
    if (encryptedJson.empty()) {
        CREATE_EVENT_LOG("Failed to encrypt JSON string");
        return std::nullopt;
    }

    const std::string requestUrl = m_url + "?data=" + encryptedJson;
    const std::string hostResponse = CurlWrapper::GetInstance()->PerformRequest(RequestType::HTTPS, requestUrl, nullptr);
    if (hostResponse.empty()) {
        CREATE_EVENT_LOG("Failed to receive a response from the Web API");
        return std::nullopt;
    }

    const std::string decryptedResponse = DataEncryption::DecryptBase64(hostResponse);
    if (decryptedResponse.empty()) {
        CREATE_EVENT_LOG("Failed to decrypt the received response");
        return std::nullopt;
    }

    if (JsonWrapper::GetInstance()->IsErrorField(decryptedResponse)) {
        CREATE_EVENT_LOG("Request have error field");
        return std::nullopt;
    }

    CREATE_EVENT_LOG("Request performed successfully");
    CREATE_EVENT_LOG("Host response: " + hostResponse + "\n");

    return decryptedResponse;
}

const AuthData& API::GetAuthDataObject() const {
    return m_authData;
}
