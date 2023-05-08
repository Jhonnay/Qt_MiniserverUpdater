#pragma once


#include <string>

class CurrentUser
{
public:
    std::string name;
    std::string uuid;
    bool isAdmin;
    bool changePassword;
    long userRights;
};
