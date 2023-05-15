#pragma once
#include "stdafx.h"
#include "CConfig.h"
#include <QThread>
#include "CUDPListener.h"
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

#define AUTO_DISCONNECTED 0
#define AUTO_IDLE 1
#define AUTO_CONNECTING 2
#define AUTO_CONNECTED 3
#define AUTO_LOADING 10
#define AUTO_LOADINGCOMPLETE 11
#define AUTO_SAVING 20
#define AUTO_SAVINGCOMPLETE 21
#define AUTO_UPDATING 30
#define AUTO_UPDATINGCOMPLETE 31
#define AUTO_NETSEARCH 40
#define AUTO_LNKSEARCH 41
#define AUTO_AIRSEARCH 42
#define AUTO_ENDSEARCH 49

#define DS_OFFLINE 0
#define DS_ONLINE 1
#define DS_SIMULATION 2
#define DS_LIVEVIEW 3

#define UPDATE_CYCLE_CHECK_CONNECTION_AND_LOAD_PROJECT 0
#define UPDATE_CYCLE_MS_UPDATE 1
#define UPDATE_CYCLE_CHECK_UPDATE 2
#define UPDATE_CYCLE_CHECK_MS_VERSIONS 3
#define UPDATE_CYCLE_DISCONNECT_CONFIG 4
#define UPDATE_CYCLE_CLOSE_CONFIG 5




class CConfigMSUpdate
{
private:
    static bool stop;
    static int updateCycleState;
    QString msIP;
    QString user;
    QString pw;
    QString projectPath = QStringLiteral("EmptyProject.Loxone");
    QString configPath;
    QString newConfigInstallerPath = nullptr;
    QString configLanguage;
    CConfig config;

public:

    CConfigMSUpdate() = default;
    CConfigMSUpdate(const QString& msIP, const QString& user, const QString& pw, const QString& configPath, const QString& configLanguage)
        : msIP(msIP),
        user(user),
        pw(pw),
        configPath(configPath),
        configLanguage(configLanguage)
    {
    }

    static int UpdateCycleState() { return updateCycleState; }
    static void SetUpdateCycleState(int value) { updateCycleState = value; }
    static bool Stop() { return stop; }
    static void SetStop(bool value) { stop = value; }
    QString MsIP() const { return msIP; }
    void setMsIP(QString value) { msIP = value; }
    QString User() const { return user; }
    void setUser(QString value) { user = value; }
    QString Pw() const { return pw; }
    void setPw(QString value) { pw = value; }
    QString ProjectPath() const { return projectPath; }
    void SetProjectPath(QString value) { projectPath = value; }
    QString ConfigPath() const { return configPath; }
    void SetConfigPath(QString value) { configPath = value; }
    QString NewConfigInstallerPath() const { return newConfigInstallerPath; }
    void SetNewConfigInstallerPath(QString value) { newConfigInstallerPath = value; }
    QString ConfigLanguage() const { return configLanguage; }
    void SetConfigLanguage(QString value) { configLanguage = value; }
    

    
    void OpenConfigLoadProject();

    void PrintConfigMsVersions(CUDPListener& udpL);

    static int getRunningConfigInstances();

    

};

