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
    int count = selectedIndexes.count();
    int progress = 1;
    int successfulUpdates = 0;
    QString progresstext = QStringLiteral("Updating %1 Miniserver(s)").arg(QString::number(count));
    emit updateStatusBarProgress(1, progresstext);


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

        progresstext = QStringLiteral("Updating %1 (%2/%3)").arg(QString::fromStdString(miniserver.getSerialNumber())).arg(QString::number(progress)).arg(QString::number(count));
        int progressInt = (progress * 100) / count -1;
        emit updateStatusBarProgress(progressInt, progresstext);

        //check if interrupted and set to "canceled" and skip FOR interation
        if (isInterruptionRequested()) {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Refresh_canceled);
            tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
            tableViewMiniserver->resizeColumnsToContents();
            tableViewMiniserver->setColumnWidth(6, 100);
            progress++;
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
        config.OpenConfigLoadProject((QThread*)this);
        int updateSuccessful = config.performMiniserverUpdate((QThread*)this);

        //Update Status to Updated
        if (updateSuccessful == 1) {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_Updated_MS_Status);
            successfulUpdates++; //if the update was successful but canceled afterwards it should count as updated. 
        }
        else if(!isInterruptionRequested() && updateSuccessful == 0){
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_retreiving_information_timeout);
        }
        else {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Refresh_canceled);
        }
        
        progress++;

        //TODO Refresh Minisever Information
        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);

        CConfig::killAllConfigs();
    }

    progresstext = QStringLiteral("Successfully updated (%1 of %2)").arg(QString::number(successfulUpdates)).arg(QString::number(count));

    if (isInterruptionRequested()) {
        emit updatingCanceled();
        progresstext = "Canceled! - " + progresstext;
        emit updateStatusBarProgress(100, progresstext);
        return;
    }
    emit updateStatusBarProgress(100, progresstext);
    emit updatingCompleted();

}
