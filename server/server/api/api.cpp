#include "api.h"

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/logsystem/logmanager/logmanager.h"

API::API(const std::string& jsonString) {
    if (jsonString.empty()) {
        CREATE_EVENT_LOG("Function call error: empty argument [" + std::string(__func__) + "]");
        return;
    }

    fillUserData(jsonString); // get main info of user (username, password etc.)
    fillMemberId(); // fills member id into AuthData for perform user hwid request after (based on parse data by getUserData method)
    fillProfileGroupId(); // fills group id into AuthData for definition access to the cheats (based on parse data by getUserData method)
}

const AuthStatus API::getAuthStatus() const {
    if (!checkUserAuthentication()) {
        CREATE_EVENT_LOG("Invalid credentials");
        return AuthStatus::AUTH_ERROR_INVALID_CREDENTIALS;
    }

    if (!checkUserHwid()) {
        CREATE_EVENT_LOG("Invalid hwid");
        return AuthStatus::AUTH_ERROR_INVALID_HWID;
    }

    if (!checkUserLicense() && m_authData.profile_group.empty()) {
        CREATE_EVENT_LOG("No license");
        return AuthStatus::AUTH_ERROR_INVALID_LICENSE;
    }

    if (!checkUserToken()) {
        CREATE_EVENT_LOG("Invalid token");
        return AuthStatus::AUTH_ERROR_INVALID_TOKEN;
    }

    return AuthStatus::AUTH_SUCCESS;
}

void API::fillUserData(const std::string& jsonString) {
    if (jsonString.empty()) {
        CREATE_EVENT_LOG("Function call error: empty argument [" + std::string(__func__) + "]");
        return;
    }

    m_authData = JsonWrapper::getInstance()->parseUserData(jsonString);
}

void API::fillProfileGroupId() {
    CREATE_EVENT_LOG("Request to get profile group");

    const std::string jsonString = JsonWrapper::getInstance()->createJsonString(
        { {"action", "groups"}, {"type", "get"} },
        { {"username", m_authData.username}, {"password", m_authData.password}, {"hwid", m_authData.hwid} }
    );

    auto requestResult = performApiRequest(jsonString);
    if (!requestResult) {
        CREATE_EVENT_LOG("Failed to get 'groups' field value");
        return;
    }

    m_authData.profile_group = JsonWrapper::getInstance()->parseParamsField(*requestResult, "groups");
}

void API::fillMemberId() {
    CREATE_EVENT_LOG("Request to get member id");

    const std::string jsonString = JsonWrapper::getInstance()->createJsonString(
        { {"action", "auth"} },
        { {"username", m_authData.username}, {"password", m_authData.password} }
    );

    auto requestResult = performApiRequest(jsonString);
    if (!requestResult) {
        CREATE_EVENT_LOG("Failed to get 'id' field value");
        return;
    }

    m_authData.member_id = JsonWrapper::getInstance()->parseParamsField(*requestResult, "id");
}

const bool API::checkUserAuthentication() const {
    CREATE_EVENT_LOG("Request to verify account availability");

    const std::string jsonString = JsonWrapper::getInstance()->createJsonString(
        { {"action", "auth"} },
        { {"username", m_authData.username}, {"password", m_authData.password} }
    );

    return performApiRequest(jsonString).has_value();
}

const bool API::checkUserHwid() const {
    CREATE_EVENT_LOG("Request to verify hardware id");

    const std::string jsonString = JsonWrapper::getInstance()->createJsonString(
        { {"action", "hwid"}, {"type", "update"} },
        { {"member_id", m_authData.member_id}, {"hwid", m_authData.hwid} }
    );

    return performApiRequest(jsonString).has_value();
}

const bool API::checkUserLicense() const {
    CREATE_EVENT_LOG("Request to verify the availability license");

    const std::string jsonString = JsonWrapper::getInstance()->createJsonString(
        { {"action", "license"}, {"type", "get"} },
        { {"username", m_authData.username}, {"password", m_authData.password} }
    );

    return performApiRequest(jsonString).has_value();
}

const bool API::checkUserToken() const {
    CREATE_EVENT_LOG("Request to verify session token");

    const std::string jsonString = JsonWrapper::getInstance()->createJsonString(
        { {"action", "session"}, {"type", "validate"} },
        { {"username", m_authData.username}, {"hwid", m_authData.hwid}, {"token", m_authData.token} }
    );

    return performApiRequest(jsonString).has_value();
}

std::optional<std::string> API::performApiRequest(const std::string& jsonString) const {
    if (jsonString.empty()) {
        CREATE_EVENT_LOG("Function call error: empty argument [" + std::string(__func__) + "]");
        return std::nullopt;
    }

    CREATE_EVENT_LOG("Detailed information on request: ");

    const std::string encryptedJson = DataEncryption::encryptBase64(jsonString);
    if (encryptedJson.empty()) {
        CREATE_EVENT_LOG("Failed to encrypt JSON string");
        return std::nullopt;
    }

    const std::string requestUrl = m_url + "?data=" + encryptedJson;
    const std::string hostResponse = CurlWrapper::getInstance()->performRequest(RequestType::HTTPS, requestUrl, nullptr);
    if (hostResponse.empty()) {
        CREATE_EVENT_LOG("Failed to receive a response from the Web API");
        return std::nullopt;
    }

    const std::string decryptedResponse = DataEncryption::decryptBase64(hostResponse);
    if (decryptedResponse.empty()) {
        CREATE_EVENT_LOG("Failed to decrypt the received response");
        return std::nullopt;
    }

    if (JsonWrapper::getInstance()->haveErrorField(decryptedResponse)) {
        CREATE_EVENT_LOG("Request have error field");
        return std::nullopt;
    }

    CREATE_EVENT_LOG("Request performed successfully");
    CREATE_EVENT_LOG("Host response: " + hostResponse + "\n");

    return decryptedResponse;
}

const AuthData& API::getAuthDataObject() const {
    return m_authData;
}
