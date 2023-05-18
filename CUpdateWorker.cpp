#include "CUpdateWorker.h"
#include "MyConstants.h"
#include "CConfigMSUpdate.h"

CUpdateWorker::CUpdateWorker(QObject *parent)
	: QThread(parent)
{}

CUpdateWorker::CUpdateWorker(QObject* parent, Qt_MiniserverTableView* tableview, Qt_Bottom_Action_Buttons* actionButtons, Qt_Statusbar* bottomStatus)
	: QThread(parent)
{
	tableViewMiniserver = tableview;
	bottom_buttons = actionButtons;
	statusbar = bottomStatus;
}

CUpdateWorker::~CUpdateWorker()
{}

void CUpdateWorker::run()
{
    const QModelIndexList selectedIndexes = tableViewMiniserver->selectionModel()->selectedRows();
    QString configPath = statusbar->getConfigExePath();

    //Update Listview for selected miniservers --> to be updated. 
    for (const QModelIndex& index : selectedIndexes)
    {
        CMiniserver miniserver = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Update_pending);
        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);
    }

    for (const QModelIndex& index : selectedIndexes)
    {
        int row = index.row();
        CMiniserver miniserver = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());

        //check if interrupted and set to "canceled" and skip FOR interation
        if (isInterruptionRequested()) {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Refresh_canceled);
            tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
            tableViewMiniserver->resizeColumnsToContents();
            tableViewMiniserver->setColumnWidth(6, 100);
            continue;
        }

        //set status to "updating" 
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_Updating_Emoji);
        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);

        //Prepare Updating Class
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

        //Load from Miniserver and Update
        config.OpenConfigLoadProject();
        config.performMiniserverUpdate();
        
        //Update Status to Updated
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_Updated_MS_Status);

        //TODO Refresh Minisever Information
        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);

        CConfig::killAllConfigs();
    }

   

    if (isInterruptionRequested()) {
        emit updatingCanceled();
        
        return;
    }

    emit updatingCompleted();

}
