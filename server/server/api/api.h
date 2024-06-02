#pragma once
#ifndef API_H
#define API_H

#include <string>
#include <optional>

#include "../../general/wrappers/jsonwrapper/jsonwrapper.h"
#include "../../general/wrappers/curlwrapper/curlwrapper.h"

enum AuthStatus {
    AUTH_INVALID_CREDENTIALS,
    AUTH_INVALID_HWID,
    AUTH_INVALID_LICENSE,
    AUTH_INVALID_TOKEN,
    AUTH_SUCCESS
};

class API {
public:
    API(const std::string& jsonString);

    const AuthStatus GetAuthStatus() const;
    const AuthData& GetAuthDataObject() const;

private:
    void GetUserCredentials(const std::string& jsonString);
    void GetProfileGroupId();
    void GetMemberId();

    const bool CheckUserAuthentication() const;
    const bool CheckUserHwid() const;
    const bool CheckUserLicense() const;
    const bool CheckUserToken() const;

    std::optional<std::string> PerformApiRequest(const std::string& jsonString) const;

private:
    AuthData m_authData; 
    const std::string m_url = "https://aceid.cc/server_api/api.php"; 
};

#endif // !API_H
