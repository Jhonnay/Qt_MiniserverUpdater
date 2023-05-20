#include "Qt_MiniserverUpdater.h"
#include "CWebService.h"
#include "MyConstants.h"
#include "CConfigMSUpdate.h"
#include <QTimer>
#include <QtConcurrent>
#include "CUpdateWorker.h"
#include "CRefreshWorker.h"
#include "Qt_CreateEditMiniserver.h"
#include "Qt_ApplicationSettings.h"

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
    updateWorker = new CUpdateWorker(this, tableViewMiniserver, bottom_buttons, statusbar);
    refreshWorker = new CRefreshWorker(this, tableViewMiniserver, bottom_buttons, statusbar);
    connectConfigWorker = new CConnectConfigWorker(this, tableViewMiniserver, bottom_buttons, statusbar);
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
    connect(refreshWorker, &CRefreshWorker::refreshCompleted, this, &Qt_MiniserverUpdater::onRefreshMiniserversFinished);
    connect(refreshWorker, &CRefreshWorker::refreshCanceled, this, &Qt_MiniserverUpdater::onRefreshCancelClicked);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonCancelClicked, this, &Qt_MiniserverUpdater::onRefreshCancelClicked);

    connect(connectConfigWorker, &CConnectConfigWorker::connectingCompleted, this, &Qt_MiniserverUpdater::onRefreshMiniserversFinished);
    connect(connectConfigWorker, &CConnectConfigWorker::connectingCanceled, this, &Qt_MiniserverUpdater::onCancelConnectConfigClicked);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonCancelClicked, this, &Qt_MiniserverUpdater::onCancelConnectConfigClicked);

    connect(refreshWorker, &CRefreshWorker::updateStatusBarProgress, statusbar, &Qt_Statusbar::updateProgress);
    connect(updateWorker, &CUpdateWorker::updateStatusBarProgress, statusbar, &Qt_Statusbar::updateProgress);

    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonAddClicked, this, &Qt_MiniserverUpdater::onAddMiniserverPressed);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonRemoveClicked, this, &Qt_MiniserverUpdater::onRemoveMiniserverPressed);
    connect(menubar, &Qt_Menubar::applicationSettingsClicked, this, &Qt_MiniserverUpdater::onApplicationSettingsClicked);
    connect(statusbar, &Qt_Statusbar::exefilepathChanged, this, &Qt_MiniserverUpdater::onConfigFilePathChanged);

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
        qDebug() << "Row: " << i << " - " << list->at(i).toString();
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

void Qt_MiniserverUpdater::setApplicationsettings(ApplicationSettings* settings)
{
    this->applicationSettings = settings;
}

void Qt_MiniserverUpdater::onApplicationSettingsClicked() {
    QString roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filePath = roamingPath + "/ApplicationSettings.json";
    qDebug() << "Application  Settings Menu clicked!";
    ApplicationSettings* settingsbefore = this->applicationSettings;

    ApplicationSettings settings = Qt_ApplicationSettings::createDialog("Application Settings", settingsbefore, nullptr);
    if (!settings.isDummyApplicationSettings()) {
        QJsonObject jsonObj;

        // Set values in the JSON object from the ApplicationSettings object
        jsonObj["BUseDefaultConfiguration"] = settings.getBUseDefaultConfiguration();
        jsonObj["StrDefaultConfigurationPath"] = QString::fromStdString(settings.getStrDefaultConfigurationPath());
        jsonObj["BUseDefaultConfig"] = settings.getBUseDefaultConfig();
        jsonObj["StrDefaultConfigPath"] = QString::fromStdString(settings.getStrDefaultConfigPath());

        QJsonDocument jsonDoc(jsonObj);

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            // Handle error opening file
            return;
        }

        file.write(jsonDoc.toJson());
        file.close();
        this->applicationSettings->setBUseDefaultConfig(settings.getBUseDefaultConfig());
        this->applicationSettings->setBUseDefaultConfiguration(settings.getBUseDefaultConfiguration());
        this->applicationSettings->setStrDefaultConfigPath(settings.getStrDefaultConfigPath());
        this->applicationSettings->setStrDefaultConfigurationPath(settings.getStrDefaultConfigurationPath());
        statusbar->setConfigExePath(QString::fromStdString(applicationSettings->getStrDefaultConfigPath()));

        for (int i = 0; i < miniservers->count(); i++) {
            QModelIndex index = tableViewMiniserver->getMiniserverModel()->index(i, 0);
            CMiniserver miniserver = miniservers->at(i);
            miniserver.setVersionColor(miniserver.calculateVersionColor(CConfig::getConfigFileVersionUnformated(QString::fromStdString(applicationSettings->getStrDefaultConfigPath()))));
            tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
            tableViewMiniserver->resizeColumnsToContents();
            tableViewMiniserver->setColumnWidth(6, 100);
        }
        
    }
    
}

void Qt_MiniserverUpdater::onConfigFilePathChanged() {
    for (int i = 0; i < miniservers->count(); i++) {
        QModelIndex index = tableViewMiniserver->getMiniserverModel()->index(i, 0);
        CMiniserver miniserver = miniservers->at(i);
        miniserver.setVersionColor(miniserver.calculateVersionColor(CConfig::getConfigFileVersionUnformated(statusbar->getConfigExePath())));
        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);
    }

}

void Qt_MiniserverUpdater::onCancelConnectConfigClicked() {
    this->connectConfigWorker->requestInterruption();
    connect(connectConfigWorker, &CConnectConfigWorker::finished, this, &Qt_MiniserverUpdater::onConnectConfigFinished);
}

void Qt_MiniserverUpdater::onUpdateStatusbarProgress(int progress, QString progresstext)
{
    emit updateStatusbarProgress(progress, progresstext);
}

void Qt_MiniserverUpdater::onAddMiniserverPressed()
{
    CMiniserver miniserver = Qt_CreateEditMiniserver::createDialog("Add Miniserver",nullptr,miniservers);
    if (!miniserver.isDummy()) {

        ////CMiniserverTableModel* model = tableViewMiniserver->getMiniserverModel();
        //
        ////int row = model->rowCount();
        ////tableViewMiniserver->getMiniserverModel()->insertRow(miniserver);
        //
        //CMiniserverTableModel* model = tableViewMiniserver->getMiniserverModel();
        //model->insertRow(miniserver);
        //
        //// Notify the view about the changes
        //int row = model->rowCount() - 1; // Index of the newly inserted row
        //QModelIndex topLeft = model->index(row, 0);
        //QModelIndex bottomRight = model->index(row, model->columnCount() - 1);
        //emit model->dataChanged(topLeft, bottomRight);

        //tableViewMiniserver->insertRow(miniserver);
        miniservers->append(miniserver);
        setMiniserverList(miniservers);
        qDebug() << "--------------------- Miniserver ADDED -------------\n" << miniserver.toString();
    }

}

void Qt_MiniserverUpdater::onRemoveMiniserverPressed()
{
    const QModelIndexList selectedIndexes = tableViewMiniserver->selectionModel()->selectedRows();
    for (const QModelIndex& index : selectedIndexes) {
        miniservers->removeAt(index.row());
        setMiniserverList(miniservers);
    }
}

void Qt_MiniserverUpdater::onConnectConfigFinished() {
    bottom_buttons->setDisabledAllExceptCancel(false);
}

void Qt_MiniserverUpdater::onRefreshCancelClicked() {
    this->refreshWorker->requestInterruption();
    connect(refreshWorker, &CRefreshWorker::finished, this, &Qt_MiniserverUpdater::onRefreshMiniserversFinished);
}


void Qt_MiniserverUpdater::onRefreshMiniserversFinished() {
    bottom_buttons->setDisabledAllExceptCancel(false);
}



void Qt_MiniserverUpdater::onRefreshClicked()
{
    bottom_buttons->setDisabledAllExceptCancel(true);
    refreshWorker->start();
    
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
    bottom_buttons->setDisabledAllExceptCancel(true);
    connectConfigWorker->setMiniserver(miniserver);
    connectConfigWorker->start();
}


