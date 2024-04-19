#pragma once
#ifndef API_H
#define API_H

#include <string>

#include "../../wrappers/jsonwrapper/jsonwrapper.h"
#include "../../wrappers/curlwrapper/curlwrapper.h"

class API
{
public:
    API(const std::string& jsonString);

    bool isAuthorized();

private:
    void getUserData(const std::string& jsonString);

    bool checkUserAuthentication();
    bool checkUserHwid();
    bool checkUserLicense();
    bool checkUserToken();

    bool performApiRequest(const std::string& jsonString);
private:
    AuthData data;
    const std::string url = "https://aceid.cc/server_api/api.php";
};

#endif // !API_H
