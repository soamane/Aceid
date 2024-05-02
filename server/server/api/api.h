#pragma once
#ifndef API_H
#define API_H

#include <string>

#include "../../general/wrappers/jsonwrapper/jsonwrapper.h"
#include "../../general/wrappers/curlwrapper/curlwrapper.h"

/*

    API class for communicating with the web server

*/

class API
{
public:
    // Constructor calls a function to obtain the client's data structure.
    API(const std::string& jsonString);

    // Returns the final authorization status
    bool isAuthorized( );

    // Returns the 'username' field from the client's data structure.
    const std::string getUsername( );

private:

    // Calls the function for parsing a JSON string into a structure
    void getUserData(const std::string& jsonString);

    /*
        Performs a request to check the presence of an account on the web server
        Return value: True - check passed / False - check failed
    */
    bool checkUserAuthentication( );

    /*
        Performs a request to verify the user's Hardware ID on the web server
        Return value: True - check passed / False - check failed
    */
    bool checkUserHwid( );

    /*
        Performs a request to check for the presence of a license on the account on the web server
        Return value: True - check passed / False - check failed
    */
    bool checkUserLicense( );

    /*
        Performs a request to verify the session token on the web server
        Return value: True - check passed / False - check failed
    */
    bool checkUserToken( );

    /*
        Performs a request to the web server using the specified JSON format string
        Return value: True - check passed / False - check failed
    */
    bool performApiRequest(const std::string& jsonString);

private:
    AuthData m_authData; // object of the client data structure
    const std::string m_url = "https://aceid.cc/server_api/api.php"; // URL of the web server
};

#endif // !API_H
