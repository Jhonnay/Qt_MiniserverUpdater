#include "CRefreshWorker.h"
#include "MyConstants.h"
#include "CConfigMSUpdate.h"
#include "CLoxAppJson.h"
#include "CWebService.h"

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

        //check if interrupted and set to "canceled" and skip FOR interation
        if (isInterruptionRequested()) {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Refresh_canceled);
            miniserver.setVersionColor("RED");
            miniserver.setMiniserverVersion("0.0.0.0");
            tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
            tableViewMiniserver->resizeColumnsToContents();
            tableViewMiniserver->setColumnWidth(6, 100);
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
        }
        else if (cloxapp.gatewayType == 0)
        {
            miniserver.setMiniserverConfiguration(MyConstants::Strings::Listview_Refresh_MS_Configuration_Standalone);
        }
        else
        {
            miniserver.setMiniserverConfiguration(MyConstants::Strings::Listview_Refresh_MS_Configuration_ClientGateway);

        }


        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);
        
    }

    if (isInterruptionRequested()) {
        emit refreshCanceled();

        return;
    }

    emit refreshCompleted();

    

}
