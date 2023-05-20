
#include "CApplicationSettings.h"



ApplicationSettings::ApplicationSettings()
{
    this->bUseDefaultConfiguration = false;
    this->strDefaultConfigurationPath = "not set";
    this->bUseDefaultConfig = false;
    this->strDefaultConfigPath = "not set";
}

ApplicationSettings::ApplicationSettings(bool bUseDefaultConfiguration, std::string strDefaultConfigurationPath, bool bUseDefaultConfig, std::string strDefaultConfigPath)
{
    this->bUseDefaultConfiguration = bUseDefaultConfiguration;
    this->strDefaultConfigurationPath = strDefaultConfigurationPath;
    this->bUseDefaultConfig = bUseDefaultConfig;
    this->strDefaultConfigPath = strDefaultConfigPath;
}

bool ApplicationSettings::getBUseDefaultConfiguration() const
{
    return bUseDefaultConfiguration;
}

void ApplicationSettings::setBUseDefaultConfiguration(bool bUseDefaultConfiguration)
{
    this->bUseDefaultConfiguration = bUseDefaultConfiguration;
}

std::string ApplicationSettings::getStrDefaultConfigurationPath() const
{
    return strDefaultConfigurationPath;
}

void ApplicationSettings::setStrDefaultConfigurationPath(const std::string& strDefaultConfigurationPath)
{
    this->strDefaultConfigurationPath = strDefaultConfigurationPath;
}

bool ApplicationSettings::getBUseDefaultConfig() const
{
    return bUseDefaultConfig;
}

void ApplicationSettings::setBUseDefaultConfig(bool bUseDefaultConfig)
{
    this->bUseDefaultConfig = bUseDefaultConfig;
}

std::string ApplicationSettings::getStrDefaultConfigPath() const
{
    return strDefaultConfigPath;
}

void ApplicationSettings::setStrDefaultConfigPath(const std::string& strDefaultConfigPath)
{
    this->strDefaultConfigPath = strDefaultConfigPath;
}


bool ApplicationSettings::isDummyApplicationSettings() {
    if (
        this->bUseDefaultConfiguration == false &&
        this->strDefaultConfigurationPath == "not set" &&
        this->bUseDefaultConfig == false &&
        this->strDefaultConfigPath == "not set"
        ) {
        return true;
    }

    return false;
}