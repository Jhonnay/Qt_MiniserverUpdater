#include "CUpdateWorker.h"
#include "MyConstants.h"
#include "CConfigMSUpdate.h"
#include "CRefreshWorker.h"
#include "CWebService.h"

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
    QModelIndexList selectedIndexes = tableViewMiniserver->selectionModel()->selectedRows();
    std::sort(selectedIndexes.begin(), selectedIndexes.end(), [](const QModelIndex& a, const QModelIndex& b) { return a.row() < b.row(); });
    emit setEnableTableview(false);
    QString configPath = statusbar->getConfigExePath();
    int count = selectedIndexes.count();
    int progress = 1;
    int successfulUpdates = 0;
    QString progresstext = QStringLiteral("⏳ Updating %1 Miniserver(s)").arg(QString::number(count));
    emit updateStatusBarProgress(1, progresstext);
    tableViewMiniserver->clearSelection();
    CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(tableViewMiniserver->model());
    if (!model) {
        return;
    }

    //Update Listview for selected miniservers --> to be updated. 
    for (const QModelIndex& index : selectedIndexes)
    {
        CMiniserver miniserver = model->m_searchText.isEmpty() ? model->miniserverlist->at(index.row()) : model->filteredMiniservers->at(index.row());
        int trueIndex = model->miniserverlist->indexOf(miniserver);
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Update_pending);
        if (!model->filteredMiniservers->empty())
            model->filteredMiniservers->replace(index.row(), miniserver);
        model->miniserverlist->replace(trueIndex, miniserver);
        //tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);
    }
    int progressInt= (progress * 100 / 2) / count;
    for (const QModelIndex& index : selectedIndexes)
    {
        int row = index.row();
        int updateSuccessful=0;
        CMiniserver miniserver = model->m_searchText.isEmpty() ? model->miniserverlist->at(index.row()) : model->filteredMiniservers->at(index.row());
        int trueIndex = model->miniserverlist->indexOf(miniserver);

        progresstext = QStringLiteral("⏳ Updating %1 (%2/%3)").arg(QString::fromStdString(miniserver.getSerialNumber())).arg(QString::number(progress)).arg(QString::number(count));
        emit updateStatusBarProgress(progressInt, progresstext);

        //check if interrupted and set to "canceled" and skip FOR interation
        if (isInterruptionRequested()) {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Refresh_canceled);
            if (!model->filteredMiniservers->empty())
                model->filteredMiniservers->replace(index.row(), miniserver);
            model->miniserverlist->replace(trueIndex, miniserver);
            tableViewMiniserver->resizeColumnsToContents();
            tableViewMiniserver->setColumnWidth(6, 100);
            progressInt = (progress * 100) / count;
            progress++;
            continue;
        }

        //set status to "updating" 
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_Updating_Emoji);
        if (!model->filteredMiniservers->empty())
            model->filteredMiniservers->replace(index.row(), miniserver);
        model->miniserverlist->replace(trueIndex, miniserver);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);

        //Prepare Updating Class
        CConfigMSUpdate config;
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
            config.setMsIP(QString::fromStdString(miniserver.getSerialNumber()));
            QString url = CWebService::getCloudDNSLink(miniserver);
            unformatedVersionString = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "dev/sys/version", "value", url);
        }

        if (unformatedVersionString != "error") {
            //Load from Miniserver and Update
            config.OpenConfigLoadProject((QThread*)this);
            updateSuccessful = config.performMiniserverUpdate((QThread*)this);
        }
        
        //Update Status to Updated
        if (updateSuccessful == 1) {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_Updated_MS_Status);
            miniserver.setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(config.miniserverVersionAfterUpdate).toStdString());
            miniserver.setVersionColor(miniserver.calculateVersionColor(config.miniserverVersionAfterUpdate));
            successfulUpdates++; //if the update was successful but canceled afterwards it should count as updated. 
            
            //if (!bottom_buttons->isHandsfreeModeEnabled()) {
            //    //block Update until User sees that the miniservers Updated.
            //    QMetaObject::invokeMethod(qApp, [&]() {
            //        // Create and show the dialog here
            //        QString message = config.getMiniserverVersions();
            //        QMessageBox::information(nullptr, "Update Information", "Miniserver(s) Versions: " + message);
            //        });
            //    
            //}
        }
        else if(!isInterruptionRequested() && updateSuccessful == 0){
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_retreiving_information_timeout);
        }
        else {
            miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Refresh_canceled);
        }

        progressInt = (progress * 100) / count;
        progress++;

        //TODO Refresh Minisever Information
        if (!model->filteredMiniservers->empty())
            model->filteredMiniservers->replace(index.row(), miniserver);
        model->miniserverlist->replace(trueIndex, miniserver);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);

        CConfig::killAllConfigs();
    }

    if (successfulUpdates == 0) {
        progresstext = QStringLiteral("Updated (%1 of %2)").arg(QString::number(successfulUpdates)).arg(QString::number(count)) + ". Updates failed. ⛔ Check Network 🚧";
    }
    else {
        progresstext = QStringLiteral("Successfully Updated (%1 of %2)").arg(QString::number(successfulUpdates)).arg(QString::number(count));
    }

    if (isInterruptionRequested()) {
        emit updatingCanceled();
        progresstext = "Canceled! ⛔ " + progresstext;
        emit updateStatusBarProgress(100, progresstext);
        emit setEnableTableview(true);
        return;
    }
    emit updateStatusBarProgress(100, progresstext);
    emit updatingCompleted();
    emit setEnableTableview(true);

}
