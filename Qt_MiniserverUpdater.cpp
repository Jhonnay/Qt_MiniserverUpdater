#include "Qt_MiniserverUpdater.h"
#include "CWebService.h"
#include "MyConstants.h"
#include "CConfigMSUpdate.h"
#include <QTimer>
#include <QtConcurrent>
#include "CUpdateWorker.h"

Qt_MiniserverUpdater::Qt_MiniserverUpdater(QWidget* parent )
    : QMainWindow(parent)
{}

Qt_MiniserverUpdater::Qt_MiniserverUpdater(QList<CMiniserver>* miniserverList, QWidget *parent )
    : QMainWindow(parent)
{
    miniservers = miniserverList;
    vBox = new QVBoxLayout(this);
    menubar = new Qt_Menubar(this);
    tableViewMiniserver = new Qt_MiniserverTableView(miniservers,this);
    bottom_buttons = new Qt_Bottom_Action_Buttons(this); 
    statusbar = new Qt_Statusbar(this);
    updateWorker = new CUpdateWorker(this, tableViewMiniserver, bottom_buttons, statusbar); //TODO 
    applicationSettings = NULL;
    

    vBox->addWidget(menubar);
    vBox->addWidget(tableViewMiniserver);
    vBox->addWidget(bottom_buttons);
    vBox->addWidget(statusbar);
    vBox->setSpacing(0);
    vBox->setContentsMargins(0, 0, 0, 0);

    centralWidget = new QWidget();
    centralWidget->setLayout(vBox);

    
    connect(tableViewMiniserver, &Qt_MiniserverTableView::ConnectConfigClicked, this, &Qt_MiniserverUpdater::onConnectConfigClicked);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonRefreshClicked, this, &Qt_MiniserverUpdater::onRefreshClicked);
    //connect(tableViewMiniserver, &Qt_MiniserverTableView::localIPTextChanged, this, &Qt_MiniserverUpdater::onLocalIPTextChanged);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonUpdateClicked, this, &Qt_MiniserverUpdater::onUpdateMiniserverClicked);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonCancelClicked, this, &Qt_MiniserverUpdater::onCancelUpdateClicked);
    connect(updateWorker, &CUpdateWorker::updatingCompleted, this, &Qt_MiniserverUpdater::onUpdateMiniserversFinished);
    connect(updateWorker, &CUpdateWorker::updatingCanceled, this, &Qt_MiniserverUpdater::onCancelUpdateClicked);



    this->setCentralWidget(centralWidget);

    //ui.setupUi(this);
}

Qt_MiniserverUpdater::~Qt_MiniserverUpdater()
{} //ist das bei MainWindow noetig?

void Qt_MiniserverUpdater::setMiniserverList(QList<CMiniserver>* list)
{
    for (int i = 0; i < list->count(); i++) {
        (*list)[i].setMiniserverStatus(MyConstants::Strings::StartUp_Listview_MS_Status);
        (*list)[i].setMiniserverVersion(MyConstants::Strings::StartUp_Listview_MS_Version);
        (*list)[i].setVersionColor("darkblue");
    }
    this->miniservers = list;
    tableViewMiniserver->setModel(new CMiniserverTableModel(list,this));
    tableViewMiniserver->resizeColumnsToContents();
    tableViewMiniserver->setColumnWidth(6, 100);
}

void Qt_MiniserverUpdater::setConfigEXEPath(QString path)
{
    statusbar->setConfigExePath(path);
}





void Qt_MiniserverUpdater::onRefreshClicked()
{
    const QModelIndexList selectedIndexes =  tableViewMiniserver->selectionModel()->selectedRows();

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

        // Get the data for the selected 
        CMiniserver miniserver = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());
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
            unformatedVersionString  = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "dev/sys/version", "value");
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
        }else if (cloxapp.gatewayType == 0)
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
        //emit tableViewMiniserver->getMiniserverModel()->dataChanged(index, index);
    }

    

    qDebug() << "Printing all miniservers after RefreshButton was clicked!";
    for (int i = 0; i < miniservers->count(); i++) {
        qDebug() << "Row: " << i << " - " << miniservers->at(i).toString();
    }

}

void Qt_MiniserverUpdater::onConnectConfig()
{


}

void Qt_MiniserverUpdater::onUpdateMiniserverClicked()
{
    
    int configCount = CConfigMSUpdate::getRunningConfigInstances();
    QString configPath = statusbar->getConfigExePath();

    if (configPath == "Current Config : not selected - double click to select") {
        QMessageBox::warning(nullptr, "Error", "No Config Exe selected!");
        return;
    }
    if (configCount != 0) {
        QString message = QString::fromStdString(MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part1) + QString::number(configCount) + QString::fromStdString(MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part2);
        QMessageBox::warning(nullptr, "Error", message);
        return;
    }

    bottom_buttons->setDisabledAllExceptCancel(true);
    updateWorker->start();

}

void Qt_MiniserverUpdater::onCancelUpdateClicked()
{
    this->updateWorker->requestInterruption();
    connect(updateWorker, &CUpdateWorker::finished, this, &Qt_MiniserverUpdater::onUpdateMiniserversFinished);
}

void Qt_MiniserverUpdater::onUpdateMiniserversFinished()
{
    bottom_buttons->setDisabledAllExceptCancel(false);
}

void Qt_MiniserverUpdater::onConnectConfigClicked(const QModelIndex& index, const CMiniserver& miniserver) {
    qDebug() << miniserver.toString();

    QString configPath = statusbar->getConfigExePath();
    CConfigMSUpdate config;
    int configCount = CConfigMSUpdate::getRunningConfigInstances();

    if (configPath == "Current Config : not selected - double click to select") {
        QMessageBox::warning(nullptr, "Error", "No Config Exe selected!");
        return;
    }

    if (configCount != 0) {
        QString message = QString::fromStdString(MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part1) + QString::number(configCount) + QString::fromStdString(MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part2);
        QMessageBox::warning(nullptr, "Error", message);
        return;
    }

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

    QtConcurrent::run([&]() {
        CConfigMSUpdate _config = CConfigMSUpdate(config.MsIP(), config.User(), config.Pw(), config.ConfigPath(), config.ConfigLanguage());
        _config.OpenConfigLoadProject();
        });

    QCoreApplication::processEvents();

}


