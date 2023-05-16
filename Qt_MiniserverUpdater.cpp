#include "Qt_MiniserverUpdater.h"
#include "CWebService.h"
#include "MyConstants.h"
#include "CConfigMSUpdate.h"
#include <QTimer>
#include <QtConcurrent>

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
        // Get the row number of the selected index
        int row = index.row();

        // Get the data for the selected row
        //CMiniserver& miniserverPointer = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());
        CMiniserver miniserver = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());
        qDebug() << "Selected row: " << miniserver.getSerialNumber().c_str();
        if (!miniserver.getLocalIP().empty()) {
            QString unformatedVersionString = CWebService::sendCommandRest_Version_Local_Gen1(miniserver, "dev/sys/version", "value");
            miniserver.setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
        }
        else {
            QString unformatedVersionString  = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "dev/sys/version", "value");
            miniserver.setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
        }
        tableViewMiniserver->model()->setData(index, QVariant::fromValue(miniserver), Qt::EditRole);
        //emit tableViewMiniserver->getMiniserverModel()->dataChanged(index, index);
        QTimer::singleShot(0, tableViewMiniserver, &Qt_MiniserverTableView::update);
    }

    
    //this->update();
    //tableViewMiniserver->update();

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
    const QModelIndexList selectedIndexes = tableViewMiniserver->selectionModel()->selectedRows();

    for (const QModelIndex& index : selectedIndexes)
    {
        int row = index.row();
        CMiniserver miniserver = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());

        QString configPath = statusbar->getConfigExePath();
        CConfigMSUpdate config;
        //auto config = std::make_shared<CConfigMSUpdate>();

        if (configPath != "Current Config : not selected - double click to select") {
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
            int configCount = CConfigMSUpdate::getRunningConfigInstances();
            if (configCount == 0) {

                QtConcurrent::run([&]() {
                    CConfigMSUpdate _config = CConfigMSUpdate(config.MsIP(), config.User(), config.Pw(), config.ConfigPath(), config.ConfigLanguage());
                    _config.OpenConfigLoadProject();
                    _config.performMiniserverUpdate();
                    });

                QCoreApplication::processEvents();

            }
            else {
                QString message = QString::fromStdString(MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part1) + QString::number(configCount) + QString::fromStdString(MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part2);
                QMessageBox::warning(nullptr, "Error", message);
            }


        }
        else
        {
            QMessageBox::warning(nullptr, "Error", "No Config Exe selected!");
        }

    }
}

void Qt_MiniserverUpdater::onConnectConfigClicked(const QModelIndex& index, const CMiniserver& miniserver) {
    qDebug() << miniserver.toString();

    QString configPath = statusbar->getConfigExePath();
    CConfigMSUpdate config;
    //auto config = std::make_shared<CConfigMSUpdate>();

    if (configPath != "Current Config : not selected - double click to select") {
        config.setUser(QString::fromStdString( miniserver.getAdminUser() ));
        config.setPw(QString::fromStdString(miniserver.getAdminPassword()));
        config.SetConfigPath(configPath);
        config.SetConfigLanguage(QString::fromStdString(miniserver.getConfigLanguage()));
        if (miniserver.getLocalIP() != "" || !miniserver.getLocalIP().empty()) {
            config.setMsIP(QString::fromStdString(miniserver.getLocalIP()));
        }
        else {
            config.setMsIP(QString::fromStdString(miniserver.getSerialNumber()));
        }
        int configCount = CConfigMSUpdate::getRunningConfigInstances();
        if (configCount  == 0) {
            //CWorkerConnectConfig worker;
            //QThread thread;
            //worker.moveToThread(&thread);
            //
            //QObject::connect(&thread, &QThread::finished, &worker, &QObject::deleteLater);
            //
            //// Execute the function in the separate thread when triggered
            //QMetaObject::invokeMethod(&worker, "processOpenConfigLoadProject", Qt::QueuedConnection, Q_ARG(CConfigMSUpdate, config));
            //
            //thread.start();
            
            QtConcurrent::run([&]() {
                CConfigMSUpdate _config = CConfigMSUpdate(config.MsIP(), config.User(), config.Pw(), config.ConfigPath(), config.ConfigLanguage());
                _config.OpenConfigLoadProject();
            });
            
            QCoreApplication::processEvents();
            //config.OpenConfigLoadProject();
            
        }
        else {
            QString message = QString::fromStdString(MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part1) + QString::number(configCount) + QString::fromStdString(MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part2);
            QMessageBox::warning(nullptr, "Error", message);
        }

        
    }
    else
    {
        QMessageBox::warning(nullptr, "Error", "No Config Exe selected!");
    }


}


