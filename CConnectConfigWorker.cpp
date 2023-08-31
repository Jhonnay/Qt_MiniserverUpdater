#include "CConnectConfigWorker.h"
#include "CConfigMSUpdate.h"
#include "CWebService.h"

CConnectConfigWorker::CConnectConfigWorker(QObject *parent)
	: QThread(parent)
{}

CConnectConfigWorker::CConnectConfigWorker(QObject* parent, Qt_MiniserverTableView* tableview, Qt_Bottom_Action_Buttons* actionButtons, Qt_Statusbar* bottomStatus)
	: QThread(parent)
{
	tableViewMiniserver = tableview;
	bottom_buttons = actionButtons;
	statusbar = bottomStatus;
}


CConnectConfigWorker::~CConnectConfigWorker()
{}

void CConnectConfigWorker::setMiniserver(const CMiniserver ms)
{
    this->miniserver = ms; 
}

CMiniserver CConnectConfigWorker::getMiniserver()
{
    return this->miniserver;
}


void CConnectConfigWorker::run() {
    chanceled = false;
    QString configPath = statusbar->getConfigExePath();
    CConfigMSUpdate config;
    tableViewMiniserver->clearSelection();
    tableViewMiniserver->clearFocus();
    
    config.setUser(QString::fromStdString(miniserver.getAdminUser()));
    config.setPw(QString::fromStdString(miniserver.getAdminPassword()));
    config.SetConfigPath(configPath);
    config.SetConfigLanguage(QString::fromStdString(miniserver.getConfigLanguage())); 
    QString unformatedVersionString;
    if (miniserver.getLocalIP() != "" || !miniserver.getLocalIP().empty()) {
        config.setMsIP(QString::fromStdString(miniserver.getLocalIP()));
        unformatedVersionString = CWebService::sendCommandRest_Version_Local_Gen1(miniserver, "dev/sys/version", "value");
    }
    else {
        QString url = CWebService::getCloudDNSLink(miniserver);
        config.setMsIP(QString::fromStdString(miniserver.getSerialNumber()));
        unformatedVersionString = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "dev/sys/version", "value", url);
    }
 
    if (unformatedVersionString != "error") {
        config.OpenConfigLoadProject(this);
    }
      
    if (isInterruptionRequested()) {
        emit connectingCanceled();
        return;
    }
    
    if (unformatedVersionString != "error") {
        emit connectingCompleted(true);
    }
    else {
        emit connectingCompleted(false);
    }
    
}
