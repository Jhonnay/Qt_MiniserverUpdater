#pragma once
#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "stdafx.h"
#include <string>


class ApplicationSettings
{
public:
    ApplicationSettings();
    ApplicationSettings(bool bUseDefaultConfiguration, std::string strDefaultConfigurationPath, bool bUseDefaultConfig, std::string strDefaultConfigPath);

    bool getBUseDefaultConfiguration() const;
    void setBUseDefaultConfiguration(bool bUseDefaultConfiguration);

    std::string getStrDefaultConfigurationPath() const;
    void setStrDefaultConfigurationPath(const std::string& strDefaultConfigurationPath);

    bool getBUseDefaultConfig() const;
    void setBUseDefaultConfig(bool bUseDefaultConfig);

    std::string getStrDefaultConfigPath() const;
    void setStrDefaultConfigPath(const std::string& strDefaultConfigPath);

private:
    bool bUseDefaultConfiguration;
    bool bUseDefaultConfig;
    std::string strDefaultConfigurationPath;
    std::string strDefaultConfigPath;
};

#endif // APPLICATIONSETTINGS_H
