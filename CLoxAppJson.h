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

    CLoxAppJson() : serialNr(""), msName(""), projectName(""), localUrl(""), remoteUrl(""), tempUnit(0), currency(""), squareMeasure(""), location(""), latitude(0.0), gatewayType(0), longitude(0.0), altitude(0), languageCode(""), heatPeriodStart(""), heatPeriodEnd(""), coolPeriodStart(""), coolPeriodEnd(""), catTitle(""), roomTitle(""), miniserverType(0), deviceMonitor(""), currentUser(CurrentUser()) {}

    CLoxAppJson(std::string serialNr_, std::string msName_, std::string projectName_, std::string localUrl_,
        std::string remoteUrl_, int tempUnit_, std::string currency_, std::string squareMeasure_, std::string location_,
        double latitude_, int gatewayType_, double longitude_, int altitude_, std::string languageCode_,
        std::string heatPeriodStart_, std::string heatPeriodEnd_, std::string coolPeriodStart_, std::string coolPeriodEnd_,
        std::string catTitle_, std::string roomTitle_, int miniserverType_, std::string deviceMonitor_,
        CurrentUser currentUser_)
    {
        serialNr = serialNr_;
        msName = msName_;
        projectName = projectName_;
        localUrl = localUrl_;
        remoteUrl = remoteUrl_;
        tempUnit = tempUnit_;
        currency = currency_;
        squareMeasure = squareMeasure_;
        location = location_;
        latitude = latitude_;
        gatewayType = gatewayType_;
        longitude = longitude_;
        altitude = altitude_;
        languageCode = languageCode_;
        heatPeriodStart = heatPeriodStart_;
        heatPeriodEnd = heatPeriodEnd_;
        coolPeriodStart = coolPeriodStart_;
        coolPeriodEnd = coolPeriodEnd_;
        catTitle = catTitle_;
        roomTitle = roomTitle_;
        miniserverType = miniserverType_;
        deviceMonitor = deviceMonitor_;
        currentUser = currentUser_;
    }
};


