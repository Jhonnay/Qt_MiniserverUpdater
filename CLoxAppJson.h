#pragma once

#include "CurrentUser.h"

class CLoxAppJson
{
public:
    std::string serialNr;
    std::string msName;
    std::string projectName;
    std::string localUrl;
    std::string remoteUrl;
    int tempUnit;
    std::string currency;
    std::string squareMeasure;
    std::string location;
    double latitude;
    int gatewayType;
    double longitude;
    int altitude;
    std::string languageCode;
    std::string heatPeriodStart;
    std::string heatPeriodEnd;
    std::string coolPeriodStart;
    std::string coolPeriodEnd;
    std::string catTitle;
    std::string roomTitle;
    int miniserverType;
    std::string deviceMonitor;
    CurrentUser currentUser;
};

