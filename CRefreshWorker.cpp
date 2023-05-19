#include "CRefreshWorker.h"
#include "MyConstants.h"
#include "CConfigMSUpdate.h"
#include "CLoxAppJson.h"
#include "CWebService.h"
#include <QStringLiteral>

CRefreshWorker::CRefreshWorker(QObject* parent, Qt_MiniserverTableView* tableview, Qt_Bottom_Action_Buttons* actionButtons, Qt_Statusbar* bottomStatus)
	: QThread(parent)
{
	tableViewMiniserver = tableview;
	bottom_buttons = actionButtons;
	statusbar = bottomStatus;
}

CRefreshWorker::~CRefreshWorker()
{}

void CRefreshWorker::run()
{
    const QModelIndexList selectedIndexes = tableViewMiniserver->selectionModel()->selectedRows();
    int count = selectedIndexes.count();
    int progress = 1;
    int successfulRetreives = 0;
    QString progresstext = QStringLiteral("Refreshing %1 Miniserver information").arg(QString::number(count));
    emit updateStatusBarProgress(1, progresstext);

    for (const QModelIndex& index : selectedIndexes)
    {
        CMiniserver miniserver = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());
        miniserver.setMiniserverStatus(MyConstants::Strings::StartUp_Listview_MS_Version);
        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);
    }
    for (const QModelIndex& index : selectedIndexes)
    {
        
        // Get the row number of the selected index
        int row = index.row();
        QString unformatedVersionString;
        QString updateLevel;
        CLoxAppJson cloxapp;
        CMiniserver miniserver = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());

        progresstext = QStringLiteral("Retreiving of %1 (%2/%3)").arg(QString::fromStdString(miniserver.getSerialNumber())).arg(QString::number(progress)).arg(QString::number(count));
        int progressInt = (progress * 100 )/ count -1;
        emit updateStatusBarProgress(progressInt, progresstext);

        //check if interrupted and set to "canceled" and skip FOR interation
        if (isInterruptionRequested()) {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Refresh_canceled);
            miniserver.setVersionColor("RED");
            miniserver.setMiniserverVersion("0.0.0.0");
            tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
            tableViewMiniserver->resizeColumnsToContents();
            tableViewMiniserver->setColumnWidth(6, 100);
            progress++;
            continue;
        }


        // Get the data for the selected 
        
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_retreiving_information);
        miniserver.setMiniserverVersion(MyConstants::Strings::StartUp_Listview_MS_Version);
        miniserver.setVersionColor("Black");
        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);

        qDebug() << "Selected row: " << miniserver.getSerialNumber().c_str();

        if (!miniserver.getLocalIP().empty()) {
            unformatedVersionString = CWebService::sendCommandRest_Version_Local_Gen1(miniserver, "dev/sys/version", "value");
            updateLevel = CWebService::sendCommandRest_Version_Local_Gen1(miniserver, "/dev/cfg/updatelevel", "value");
            cloxapp = CWebService::sendCommandRest_LoxAppJson_Local_Gen1(miniserver, "data/LoxAPP3.json");
        }
        else {
            unformatedVersionString = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "dev/sys/version", "value");
            updateLevel = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "/dev/cfg/updatelevel", "value");
            cloxapp = CWebService::sendCommandRest_LoxAppJson_Remote_Cloud(miniserver, "data/LoxAPP3.json");
        }

        if (updateLevel.contains('"')) {
            updateLevel = updateLevel.left(updateLevel.indexOf('"'));
        }
        miniserver.setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_retreiving_information_successfull);
        miniserver.setUpdatelevel(updateLevel.toStdString());
        miniserver.setMiniserverProject(cloxapp.projectName + "/" + cloxapp.localUrl);
        if (miniserver.getMiniserverVersion() == "0.0.0.0") {
            miniserver.setMiniserverConfiguration(MyConstants::Strings::Listview_Refresh_MS_Configuration_Error);
            miniserver.setUpdatelevel(MyConstants::Strings::Listview_Refresh_MS_Configuration_Error);
            miniserver.setVersionColor("RED");
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_retreiving_information_timeout);
        }
        else if (cloxapp.gatewayType == 0)
        {
            miniserver.setMiniserverConfiguration(MyConstants::Strings::Listview_Refresh_MS_Configuration_Standalone);
            successfulRetreives++;
        }
        else
        {
            miniserver.setMiniserverConfiguration(MyConstants::Strings::Listview_Refresh_MS_Configuration_ClientGateway);
            successfulRetreives++;
        }

        progress++;

        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);
        
    }
    progresstext = QStringLiteral("Successfully retreived (%1 of %2)").arg(QString::number(successfulRetreives)).arg(QString::number(count));
    if (isInterruptionRequested()) {
        emit refreshCanceled();
        progresstext = "Canceled! - " + progresstext;
        emit updateStatusBarProgress(100, progresstext);
        return;
    }
    
    emit updateStatusBarProgress(100, progresstext);
    emit refreshCompleted();
    

    

}
