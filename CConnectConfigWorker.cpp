#include "CConnectConfigWorker.h"
#include "CConfigMSUpdate.h"

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


void CConnectConfigWorker::run() {
    QString configPath = statusbar->getConfigExePath();
    CConfigMSUpdate config;
    config.setUser(QString::fromStdString(miniserver.getAdminUser()));
    config.setPw(QString::fromStdString(miniserver.getAdminPassword()));
    config.SetConfigPath(configPath);
    config.SetConfigLanguage(QString::fromStdString(miniserver.getConfigLanguage()));
    if (miniserver.getLocalIP() != "" || !miniserver.getLocalIP().empty()) {
        config.setMsIP(QString::fromStdString(miniserver.getLocalIP()));
    }
    else {
        config.setMsIP(QString::fromStdString(miniserver.getSerialNumber()));
    }

    
        
    config.OpenConfigLoadProject(this);
       
    if (isInterruptionRequested()) {
        emit connectingCanceled();

        return;
    }

    emit connectingCompleted();
}
