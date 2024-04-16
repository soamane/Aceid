#pragma once
#ifndef API_H
#define API_H

#include <string>

#include "../jsonwrapper/jsonwrapper.h"
#include "../curlwrapper/curlwrapper.h"

class API
{
public:
    API(const std::string& jsonString);
    
    bool checkUserAuthentication();
    bool checkUserHwid();

private:
    void getClientAuthData(const std::string& jsonString);

private:
    AuthData data;
    const std::string source = "https://aceid.cc/server_api/api.php";
};

#endif // !API_H
