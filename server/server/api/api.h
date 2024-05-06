#pragma once
#ifndef API_H
#define API_H

#include <string>
#include <optional>

#include "../../general/wrappers/jsonwrapper/jsonwrapper.h"
#include "../../general/wrappers/curlwrapper/curlwrapper.h"

class API {
public:
    API(const std::string& jsonString);

    bool isAuthorized();

    const std::string getUsername();

private:

    void getUserData(const std::string& jsonString);

    void getProfileGroup();

    void getMemberId();

    bool checkUserAuthentication();

    bool checkUserHwid();

    bool checkUserLicense();

    bool checkUserToken();

    std::optional<const std::string> performApiRequest(const std::string& jsonString);

private:
    AuthData m_authData; 
    const std::string m_url = "https://aceid.cc/server_api/api.php"; 
};

#endif // !API_H
