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

    
    CurrentUser() : name(""), uuid(""), isAdmin(false), changePassword(false), userRights(0) {}


    CurrentUser(std::string name,
        std::string uuid,
        bool isAdmin,
        bool changePassword,
        long userRights) 
    {
        this->name = name;
        this ->uuid = uuid;
        this->isAdmin = isAdmin;
        this->changePassword = changePassword;
        this->userRights = userRights;
    }
};
