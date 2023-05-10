#pragma once
#include "CConfig.h"

class CMiniserver
{
private:
    std::string serialNumber;
    std::string adminUser;
    std::string adminPassword;
    std::string miniserverVersion;
    std::string miniserverStatus;
    std::string updatelevel;
    std::string versionColor;
    std::string miniserverProject;
    std::string miniserverConfiguration;
    std::string localIP;
    std::string configLanguage;

public:
    //Constructor
    CMiniserver(const std::string& serialNumber, const std::string& adminUser,
        const std::string& adminPassword, const std::string& miniserverVersion,
        const std::string& miniserverStatus, const std::string& updatelevel,
        const std::string& versionColor, const std::string& miniserverProject,
        const std::string& miniserverConfiguration, const std::string& localIP,
        const std::string& configLanguage);


    // Getter and Setter for serialNumber
    std::string getSerialNumber() const;
    void setSerialNumber(const std::string& serialNumber);

    // Getter and Setter for adminUser
    std::string getAdminUser() const;
    void setAdminUser(const std::string& adminUser);

    // Getter and Setter for adminPassword
    std::string getAdminPassword() const;
    void setAdminPassword(const std::string& adminPassword);

    // Getter and Setter for miniserverVersion
    std::string getMiniserverVersion() const;
    void setMiniserverVersion(const std::string& miniserverVersion);

    // Getter and Setter for miniserverStatus
    std::string getMiniserverStatus() const;
    void setMiniserverStatus(const std::string& miniserverStatus);

    // Getter and Setter for updatelevel
    std::string getUpdatelevel() const;
    void setUpdatelevel(const std::string& updatelevel);

    // Getter and Setter for versionColor
    std::string getVersionColor() const;
    void setVersionColor(const std::string& versionColor);

    // Getter and Setter for miniserverProject
    std::string getMiniserverProject() const;
    void setMiniserverProject(const std::string& miniserverProject);

    // Getter and Setter for miniserverConfiguration
    std::string getMiniserverConfiguration() const;
    void setMiniserverConfiguration(const std::string& miniserverConfiguration);

    // Getter and Setter for localIP
    std::string getLocalIP() const;
    void setLocalIP(const std::string& localIP);

    //Getter and Setter for configLanguage
    std::string getConfigLanguage() const;
    void setConfigLanguage(const std::string& language);

    //toString Function
    std::string toString() const;
};