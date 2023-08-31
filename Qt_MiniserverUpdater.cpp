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
#include <algorithm>
#include <cstdlib>
#include "CFileParser.cpp"
#include "CLoxoneApp.h"
//#include <QWebEngineView>

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
    bottom_buttons->setDisabledAllExceptCancelAdd(true);
    statusbar = new Qt_Statusbar(this);
    updateWorker = new CUpdateWorker(this, tableViewMiniserver, bottom_buttons, statusbar);
    refreshWorker = new CRefreshWorker(this, tableViewMiniserver, bottom_buttons, statusbar);
    connectConfigWorker = new CConnectConfigWorker(this, tableViewMiniserver, bottom_buttons, statusbar);
    downloadProgFolderWorker = new CDownloadProgFolderWorker(this);
    applicationSettings = NULL;
    actionDeselectAll = new QAction("Deselect All", this);
    actionRefreshSelected = new QAction("Refresh Selected", this);
    actionRemoveMiniserverWithDelete = new QAction("Remove selected", this);
    checkUpdater = new CUpdateChecker(this);
    searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Search...");
    searchField->setVisible(false);  //invisible on startup
    searchField->setContentsMargins(10, 0, 10, 10);
    
    actionDeselectAll->setShortcut(QKeySequence("Ctrl+D"));
    actionRefreshSelected->setShortcut(QKeySequence("Ctrl+R"));
    actionRemoveMiniserverWithDelete->setShortcut(QKeySequence::Delete);
    addAction(actionRemoveMiniserverWithDelete);
    addAction(actionDeselectAll);
    addAction(actionRefreshSelected);
    vBox->addWidget(menubar);
    vBox->addWidget(tableViewMiniserver);
    vBox->addWidget(bottom_buttons);
    vBox->addWidget(searchField);
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

    connect(connectConfigWorker, &CConnectConfigWorker::connectingCompleted, this, &Qt_MiniserverUpdater::onConnectConfigFinished);
    connect(connectConfigWorker, &CConnectConfigWorker::connectingCanceled, this, &Qt_MiniserverUpdater::onCancelConnectConfigClicked);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonCancelClicked, this, &Qt_MiniserverUpdater::onCancelConnectConfigClicked);

    connect(refreshWorker, &CRefreshWorker::updateStatusBarProgress, statusbar, &Qt_Statusbar::updateProgress);
    connect(refreshWorker, &CRefreshWorker::setEnableTableview, tableViewMiniserver, &Qt_MiniserverTableView::setEnabledTableView);
    connect(updateWorker, &CUpdateWorker::updateStatusBarProgress, statusbar, &Qt_Statusbar::updateProgress);
    connect(updateWorker, &CUpdateWorker::setEnableTableview, tableViewMiniserver, &Qt_MiniserverTableView::setEnabledTableView);

    connect(tableViewMiniserver, &Qt_MiniserverTableView::enabledStateChanged, menubar, &Qt_Menubar::updateFileMenuState);
    connect(tableViewMiniserver, &Qt_MiniserverTableView::downloadProgFolderPressed, this, &Qt_MiniserverUpdater::onDownloadProgFolder);
    connect(tableViewMiniserver, &Qt_MiniserverTableView::mySelectionChanged, this, &Qt_MiniserverUpdater::onSelectionChanged);
    connect(downloadProgFolderWorker, &CDownloadProgFolderWorker::updateStatusBarProgress, statusbar, &Qt_Statusbar::updateProgress);

    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonAddClicked, this, &Qt_MiniserverUpdater::onAddMiniserverPressed);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonRemoveClicked, this, &Qt_MiniserverUpdater::onRemoveMiniserverPressed);
    connect(menubar, &Qt_Menubar::applicationSettingsClicked, this, &Qt_MiniserverUpdater::onApplicationSettingsClicked);
    connect(statusbar, &Qt_Statusbar::exefilepathChanged, this, &Qt_MiniserverUpdater::onConfigFilePathChanged);

    connect(menubar, &Qt_Menubar::saveClicked, this, &Qt_MiniserverUpdater::onSaveFileClicked);
    connect(menubar, &Qt_Menubar::openClicked, this, &Qt_MiniserverUpdater::onOpenFileClicked);
    connect(menubar, &Qt_Menubar::newClicked, this, &Qt_MiniserverUpdater::onNewFileClicked);
    connect(menubar, &Qt_Menubar::startAppClicked, this, &Qt_MiniserverUpdater::onStartAppClicked);
    connect(menubar, &Qt_Menubar::debugAppClicked, this, &Qt_MiniserverUpdater::onStartDebugAppClicked);
    connect(menubar, &Qt_Menubar::killAppClicked, this, &Qt_MiniserverUpdater::onKillLoxoneAppClicked);
    connect(menubar, &Qt_Menubar::versionClicked, this, &Qt_MiniserverUpdater::onVersionClicked);    
    connect(menubar, &Qt_Menubar::changelogClicked, this, &Qt_MiniserverUpdater::onChangelogClicked);
    connect(menubar, &Qt_Menubar::checkVersionClicked, this, &Qt_MiniserverUpdater::onCheckNewVersionClicked);
    connect(menubar, &Qt_Menubar::help, this, &Qt_MiniserverUpdater::onHelp);

    connect(actionDeselectAll, &QAction::triggered, this, &Qt_MiniserverUpdater::onDeselectAll);
    connect(actionRefreshSelected, &QAction::triggered, this, &Qt_MiniserverUpdater::onRefreshSelected);
    connect(actionRemoveMiniserverWithDelete, &QAction::triggered, this, &Qt_MiniserverUpdater::onRemoveMiniserverPressed);

    connect(searchField, &QLineEdit::textChanged, this, &Qt_MiniserverUpdater::handleSearchTextChanged);

    this->setCentralWidget(centralWidget);
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
    tableViewMiniserver->disconnect(tableViewMiniserver->selectionModel(), &QItemSelectionModel::selectionChanged, tableViewMiniserver, &Qt_MiniserverTableView::handleSelectionChanged);
    tableViewMiniserver->setModel(new CMiniserverTableModel(list,this));
    tableViewMiniserver->connect(tableViewMiniserver->selectionModel(), &QItemSelectionModel::selectionChanged, tableViewMiniserver, &Qt_MiniserverTableView::handleSelectionChanged);
    tableViewMiniserver->resizeColumnsToContents();
    tableViewMiniserver->setColumnWidth(6, 100);

    CMiniserverTableModel* model = tableViewMiniserver->getMiniserverModel();
    qDebug() << "Address of Model after setting Miniservers: " << model;
}

void Qt_MiniserverUpdater::PrintupdatedMiniserverList()
{
    CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(tableViewMiniserver->model());
    if (!model) {
        return;
    }
    for (int i = 0; i < model->miniserverlist->count(); i++) {
        qDebug() << "Row: " << i << " - " << model->miniserverlist->at(i).toString();
    }
}

void Qt_MiniserverUpdater::setConfigEXEPath(QString path)
{
    statusbar->setConfigExePath(path);
}

void Qt_MiniserverUpdater::setApplicationsettings(ApplicationSettings* settings)
{
    QString configpath = QString::fromStdString(settings->getStrDefaultConfigPath());
    QString miniserverListpath = QString::fromStdString(settings->getStrDefaultConfigurationPath());

    //TODO::all off this can be prevented, if I make the Aplication Settings Textboxes not editable. 
    //TODO: Check function (also used in Application Settings Dialog)
    bool miniserverlistValid = QFileInfo(miniserverListpath).exists();
    bool configexeValid = QFileInfo(configpath).exists();
    if (miniserverListpath == "not set") { miniserverlistValid = true; }
    if (configpath == "not set") { configexeValid = true; };

    if (!miniserverlistValid || !configexeValid) {
        QMessageBox::warning(nullptr, "Warning", QString::fromStdString(MyConstants::Strings::MessageBox_ApplicationSettingsPaths_not_correct));
        this->applicationSettings = new ApplicationSettings();
        return; 

    }
    this->applicationSettings = settings;
}

void Qt_MiniserverUpdater::setApplicationVersion(QString version)
{
    this->applicationVersion = version;
    this->checkUpdater->setApplicationVersion(version);
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

void Qt_MiniserverUpdater::onSaveFileClicked()
{
    // Get the default save directory path
    QString defaultDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // Show a file dialog to select the save file path
    QString filePath = QFileDialog::getSaveFileName(this, "Save Miniserver JSON", defaultDirPath, "JSON Files (*.json)");

    // Check if the user selected a file
    if (!filePath.isEmpty()) {
        // Save the Miniservers to the selected JSON file
        FileParser::saveMiniserverJsonFile(*miniservers, filePath);
    }
}

void Qt_MiniserverUpdater::onOpenFileClicked()
{
    // Get the default save directory path
    QString defaultDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // Show a file dialog to select the save file path
    QString filePath = QFileDialog::getOpenFileName(this, "Opne Miniserver JSON", defaultDirPath, "JSON Files (*.json)");

    // Check if the user selected a file
    if (!filePath.isEmpty()) {
        // Save the Miniservers to the selected JSON file
        QList<CMiniserver> list = FileParser::parseMiniserverJsonFile(filePath);
        CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(tableViewMiniserver->model());
        if (!model) {
            return;
        }
        model->miniserverlist->clear();
        for (int i = 0; i < list.size(); i++) {
            list[i].setMiniserverStatus(MyConstants::Strings::StartUp_Listview_MS_Status);
            list[i].setMiniserverVersion(MyConstants::Strings::StartUp_Listview_MS_Version);
            list[i].setVersionColor("darkblue");
            model->miniserverlist->append(list.at(i));
        }
        tableViewMiniserver->clearSelection();
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->model()->layoutChanged();
        clearSearch();
    }
}

void Qt_MiniserverUpdater::onNewFileClicked()
{
    QMessageBox msgBox(QMessageBox::Question, "New Miniserver List", QString::fromUtf8(MyConstants::Strings::MessageBox_AskUser_On_New_File), QMessageBox::Yes | QMessageBox::No, this);

    // if the user clicked yes
    if (msgBox.exec() == QMessageBox::Yes) {
        CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(tableViewMiniserver->model());
        if (!model) {
            return;
        }
        model->miniserverlist->clear();
        clearSearch();
        tableViewMiniserver->model()->layoutChanged();
        PrintupdatedMiniserverList();
    }
}

void Qt_MiniserverUpdater::onStartAppClicked()
{
    int runningAppInstances = CLoxoneApp::getRunningLoxoneApps();
    if (runningAppInstances > 0) {
        QMessageBox::warning(nullptr, "App already open", QString::fromStdString(MyConstants::Strings::MessageBox_App_already_open));
        return;
    }

    QString exeFilePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    int index = exeFilePath.lastIndexOf('/');
    if (index != -1) {
        exeFilePath = exeFilePath.left(index);
        exeFilePath += QString::fromStdString(MyConstants::Strings::Path_Folder_for_Loxone_App);
        QProcess process = QProcess();
        process.startDetached(exeFilePath);
    }
}

void Qt_MiniserverUpdater::onStartDebugAppClicked()
{
    int runningAppInstances = CLoxoneApp::getRunningLoxoneApps();
    if (runningAppInstances > 0) {

        QMessageBox::warning(nullptr, "App already open", QString::fromStdString(MyConstants::Strings::MessageBox_App_already_open));

        return;
    }

    QString exeFilePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    int index = exeFilePath.lastIndexOf('/');
    if (index != -1) {
        exeFilePath = exeFilePath.left(index);
        exeFilePath += QString::fromStdString(MyConstants::Strings::Path_Folder_for_Loxone_App);
        QProcess process = QProcess();
        QStringList arguments;
        arguments << "--debug";
        qDebug() << arguments;
        process.startDetached(exeFilePath, arguments);
    }
}

void Qt_MiniserverUpdater::onKillLoxoneAppClicked()
{
    CLoxoneApp::killAllRunningApps();
}

void Qt_MiniserverUpdater::onVersionClicked()
{
    QString message = "Current Version: " + applicationVersion;
    QMessageBox::information(nullptr, "Version", message);
}

void Qt_MiniserverUpdater::onChangelogClicked()
{
    QString changelogFilePath = QDir::currentPath() + "/Changelog.txt";
    if (QFile::exists(changelogFilePath))
    {
        QProcess::startDetached("notepad.exe", QStringList() << changelogFilePath);
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", QString::fromStdString(MyConstants::Strings::MessageBox_Changelog_Cannot_be_opened));
    }
}

void Qt_MiniserverUpdater::onDownloadProgFolder(CMiniserver ms)
{
    this->downloadProgFolderWorker->setMiniserver(ms);
    this->downloadProgFolderWorker->start();
    connect(downloadProgFolderWorker, &CDownloadProgFolderWorker::finished, this, &Qt_MiniserverUpdater::onDonwloadProgFolderFinished);

}

void Qt_MiniserverUpdater::onDonwloadProgFolderFinished() {
    bottom_buttons->setDisabledAddMiniserverButton(false);
}

void Qt_MiniserverUpdater::onDeselectAll()
{
    tableViewMiniserver->clearSelection();
}

void Qt_MiniserverUpdater::onCheckNewVersionClicked()
{
    checkUpdater->CheckUpdate_and_install_if_user_wants(true);
}

void Qt_MiniserverUpdater::onHelp()
{
    //QDialog* dialog = new QDialog();
    //QVBoxLayout* layout = new QVBoxLayout(dialog);
    //QWebEngineView* webView = new QWebEngineView(dialog);
    //layout->addWidget(webView);
    //dialog->setWindowTitle("Documentation");
    //
    //// Load the local HTML file
    //QString filePath = "/resources/Documentation.html";
    //QUrl url = QUrl::fromLocalFile(QDir::currentPath() + filePath);
    //qDebug() << url.path();
    //webView->load(url);
    //dialog->exec();
    //
    //delete dialog;
    
    QString filePath = "resources/Documentation.html";
    QUrl url = QUrl::fromLocalFile(filePath);
    QDesktopServices::openUrl(QUrl(url));

}

void Qt_MiniserverUpdater::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    if (tableViewMiniserver->selectionModel()->selectedRows().count() == 0) {
        bottom_buttons->setDisabledAllExceptCancelAdd(true);
    }
    else {
        if (!connectConfigWorker->isRunning()) {
            bottom_buttons->setDisabledAllExceptCancelAdd(false);
        }
    }
    if (!updateWorker->isRunning() &&
        !refreshWorker->isRunning() &&
        !downloadProgFolderWorker->isRunning() &&
        !connectConfigWorker->isRunning()) {
        CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(tableViewMiniserver->model());
        if (!model) {
            return;
        }
        if (model->m_searchText.isEmpty()) {
            statusbar->updateProgress(100, QString::number(tableViewMiniserver->selectionModel()->selectedRows().count()) + " of " + QString::number(miniservers->count()) + " selected");
        }
        else {
            statusbar->updateProgress(100, QString::number(tableViewMiniserver->selectionModel()->selectedRows().count()) + " of " + QString::number(model->filteredMiniservers->count()) + " filtered selected");
        }
        
    }
    else {
        bottom_buttons->setDisabledAddMiniserverButton(true);
    }
    qDebug() << "selection changed: " + QString::number(tableViewMiniserver->selectionModel()->selectedRows().count());
}

void Qt_MiniserverUpdater::onRefreshSelected()
{
    onRefreshClicked();
}

void Qt_MiniserverUpdater::onCancelConnectConfigClicked() {
    this->connectConfigWorker->requestInterruption();
    connect(connectConfigWorker, &CConnectConfigWorker::finished, this, &Qt_MiniserverUpdater::onConnectConfigCancelFinished);
}

void Qt_MiniserverUpdater::onConnectConfigCancelFinished() {
    tableViewMiniserver->setEnabled(true);
    statusbar->updateProgress(100, tr("Connecting canceled 🚧"));
}

void Qt_MiniserverUpdater::onUpdateStatusbarProgress(int progress, QString progresstext)
{
    emit updateStatusbarProgress(progress, progresstext);
}

void Qt_MiniserverUpdater::setStatusbarProgress(int progress, QString progresstext)
{
    statusbar->updateProgress(progress, progresstext);
}

void Qt_MiniserverUpdater::checkVersionOnStartup()
{
    checkUpdater->CheckUpdate_and_install_if_user_wants(false);
}

void Qt_MiniserverUpdater::onAddMiniserverPressed()
{
    CMiniserver miniserver = Qt_CreateEditMiniserver::createDialog("Add Miniserver",nullptr,miniservers);
    if (!miniserver.isDummy()) {
        CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(tableViewMiniserver->model());
        if (!model) {
            return;
        }
        miniserver.setMiniserverStatus(MyConstants::Strings::StartUp_Listview_MS_Status);
        miniserver.setMiniserverVersion(MyConstants::Strings::StartUp_Listview_MS_Version);
        miniserver.setVersionColor("darkblue");

        if (!searchField->text().isEmpty()) {
            model->miniserverlist->append(miniserver);
            if (miniserver.matchesSearchFilter(searchField->text())); {
                model->filteredMiniservers->append(miniserver);
            }
        }
        else {
            model->miniserverlist->append(miniserver);
        }

        qDebug() << "--------------------- Miniserver ADDED -------------\n" << miniserver.toString();
        tableViewMiniserver->model()->layoutChanged();
        tableViewMiniserver->clearSelection();
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);
        
        if (!searchField->text().isEmpty()) {
            tableViewMiniserver->selectRow(model->filteredMiniservers->size() - 1);
        }
        else {
            tableViewMiniserver->selectRow(model->miniserverlist->size() - 1);
        }
        tableViewMiniserver->scrollToBottom();
    }
    PrintupdatedMiniserverList();
}

void Qt_MiniserverUpdater::clearSearch()
{
    qDebug() << "Filter deactivated with STRG+F: ";
    searchField->clear();
    searchField->clearFocus();
    handleSearchTextChanged(""); // Reset the filter when search field is hidden
    tableViewMiniserver->resizeColumnsToContents();
    tableViewMiniserver->setColumnWidth(6, 100);
}

void Qt_MiniserverUpdater::onRemoveMiniserverPressed()
{
    CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(tableViewMiniserver->model());
    if (!model) {
        return;
    }

    QModelIndexList selectedIndexes = tableViewMiniserver->selectionModel()->selectedRows();
    
    QModelIndexList indexlist = tableViewMiniserver->selectionModel()->selectedRows();
    std::sort(selectedIndexes.begin(), selectedIndexes.end(), [](const QModelIndex& a, const QModelIndex& b) { return a.row() > b.row(); });
    

    for (const QModelIndex& index : selectedIndexes) {
       
        if (!model->filteredMiniservers->empty()) {
            model->miniserverlist->removeAt(model->miniserverlist->indexOf(model->filteredMiniservers->at(index.row())));
            model->filteredMiniservers->removeAt(index.row());
            model->removeRow(index.row(), index);
        }
        else {
            model->miniserverlist->removeAt(index.row());
            model->removeRow(index.row(),index);
        }
    }
    tableViewMiniserver->model()->layoutChanged();
    tableViewMiniserver->clearSelection();
    qDebug() << "--------------------- Miniserver(s) REMOVED -------------";
    PrintupdatedMiniserverList();
}

void Qt_MiniserverUpdater::onConnectConfigFinished(bool sucessfull) {
    if (sucessfull) {
        qDebug() << "---------------------- Connected to Miniserver successfully --- Finished -------";
        tableViewMiniserver->setEnabled(true);
        statusbar->updateProgress(100, tr("Connecting finished 🎉"));
    }
    else {
        qDebug() << "---------------------- Connected to Miniserver NOT successfull --- Finished -------";
        tableViewMiniserver->setEnabled(true);
        statusbar->updateProgress(100, "Connecting to " + QString::fromStdString(connectConfigWorker->getMiniserver().getSerialNumber()) + " failed. ⛔ Check Network 🚧");
    }

    bottom_buttons->setDisabledAddMiniserverButton(false);
}

void Qt_MiniserverUpdater::onRefreshCancelClicked() {
    this->refreshWorker->requestInterruption();
    connect(refreshWorker, &CRefreshWorker::finished, this, &Qt_MiniserverUpdater::onRefreshMiniserversFinished);
}


void Qt_MiniserverUpdater::onRefreshMiniserversFinished() {
    bottom_buttons->setDisabledAddMiniserverButton(false);
}


void Qt_MiniserverUpdater::onRefreshClicked()
{
    if (tableViewMiniserver->selectionModel()->selectedRows().count() == 0) {
        qDebug() << "No Miniserver are selected for Refreshing!";
        return;
    }
    bottom_buttons->setDisabledAllExceptCancelAdd(true);
    refreshWorker->start();
    
    qDebug() << "Printing all miniservers after RefreshButton was clicked!";
    PrintupdatedMiniserverList();
}

void Qt_MiniserverUpdater::onConnectConfig()
{

}

void Qt_MiniserverUpdater::onUpdateMiniserverClicked()
{
    if (tableViewMiniserver->selectionModel()->selectedRows().count() == 0) {
        qDebug() << "No Miniserver are selected for Update!";
        return;
    }
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

    bottom_buttons->setDisabledAllExceptCancelAdd(true);
    updateWorker->start();

}

void Qt_MiniserverUpdater::onCancelUpdateClicked()
{
    this->updateWorker->requestInterruption();
    connect(updateWorker, &CUpdateWorker::finished, this, &Qt_MiniserverUpdater::onUpdateMiniserversFinished);
}

void Qt_MiniserverUpdater::onUpdateMiniserversFinished()
{
    bottom_buttons->setDisabledAddMiniserverButton(false);
}

void Qt_MiniserverUpdater::onConnectConfigClicked(const QModelIndex& index, const CMiniserver& miniserver) {
    qDebug() << miniserver.toString();
    if (connectConfigWorker->isRunning()) {
        return;
    }

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

    statusbar->updateProgress(50, "Connecting to " + QString::fromStdString(miniserver.getSerialNumber()));
    tableViewMiniserver->setEnabled(false);
    bottom_buttons->setDisabledAllExceptCancelAdd(true);
    connectConfigWorker->setMiniserver(miniserver);
    connectConfigWorker->start();
    
}


void Qt_MiniserverUpdater::handleSearchTextChanged(const QString& searchText)
{
    //qDebug() << "Search Text changed: " << searchText;
    //CMiniserverTableModel* model = tableViewMiniserver->getMiniserverModel();
    //qDebug() << "Address of Model when searching: " << model;
    ////tableViewMiniserver->getMiniserverModel()->setSearchText(searchText);
    //qDebug() << "Address of simpleModel when searching: " << tableViewMiniserver->model();

    CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(tableViewMiniserver->model());
    if (model) {
        model->setSearchText(searchText);
        tableViewMiniserver->resizeColumnsToContents();
        tableViewMiniserver->setColumnWidth(6, 100);
        if (searchText.isEmpty()) {
            onSelectionChanged(QItemSelection(), QItemSelection()); //trigger manual selection change to switch status text from filtered to unfiltered message
        }
    }
    tableViewMiniserver->viewport()->repaint();
}

// Handle Ctrl+F key press event to show/hide the search field
void Qt_MiniserverUpdater::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F && (event->modifiers() & Qt::ControlModifier)) {
        if (!searchField->isVisible()) {
            searchField->setHidden(false);
        }

        if (searchField->isVisible() && searchField->hasFocus() && searchField->text().isEmpty()) {
            searchField->setHidden(true);
            clearSearch();
        }

        if (searchField->isVisible() && !searchField->hasFocus()) {
            searchField->setFocus();
            searchField->selectAll();
        }
    }
    else {
        QWidget::keyPressEvent(event);
    }
}

