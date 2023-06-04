#include "stdafx.h"
#include "Qt_Miniserver_Listview.h"
#include <QtWidgets/QApplication>
#include <stdio.h>
#include <iostream>
#include "CMiniserver.h"
#include "Qt_Bottom_Action_Buttons.h"
#include "Qt_Menubar.h"
#include "Qt_Statusbar.h"
#include "Qt_MiniserverUpdater.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include "CApplicationSettings.h"
#include "CWebService.h"
#include "CLoxAppJson.h"

#include "CFileParser.cpp"
#include <QtConcurrent>

#define VERSION "0.1.7"


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    std::cout << "Hello, World!" << std::endl;

    QList<CMiniserver> miniservers;
    //MainWindow
    Qt_MiniserverUpdater* mainwindow = new Qt_MiniserverUpdater(&miniservers, nullptr);
    mainwindow->setApplicationVersion(QString::fromStdString(VERSION));
    QString applicationSettingsPath = FileParser::checkIfApplicationSettingsExist("ApplicationSettings.json");
    ApplicationSettings applicationSettings;
    if (!applicationSettingsPath.isEmpty()) {
        applicationSettings = FileParser::parseApplicationSettingsJsonFile(applicationSettingsPath);
        mainwindow->setApplicationsettings(&applicationSettings);
        if (applicationSettings.getBUseDefaultConfig()) {
            mainwindow->setConfigEXEPath(QString::fromStdString(applicationSettings.getStrDefaultConfigPath()));
        }
        if (applicationSettings.getBUseDefaultConfiguration()) {
            miniservers = FileParser::parseMiniserverJsonFile(QString::fromStdString(applicationSettings.getStrDefaultConfigurationPath()));
            mainwindow->setMiniserverList(&miniservers);
        }
        mainwindow->setStatusbarProgress(100, "Applications Settings loaded");
    }
    else {
        applicationSettings =  ApplicationSettings();
        mainwindow->setApplicationsettings(&applicationSettings);
        mainwindow->setStatusbarProgress(100, "Applications Settings NOT loaded");
    }

    
    mainwindow->checkVersionOnStartup();
    //mainWindow.setCentralWidget(centralWidget);
    mainwindow->setWindowTitle("Miniserver Updater (c) Musat Version " + QString::fromStdString(VERSION));
    mainwindow->resize(1000, 600);
    mainwindow->show();

    return a.exec();
}






//WebServices Test
//CMiniserver testkofferZuhause = CMiniserver("504F94A06212", "admin", "admin!", "TBD", "TBD", "TBD", "Black", "TBD", "TBD", "MS6212","5");
//QString cloudDNSLink = CWebService::getCloudDNSLink(miniservers[0]);
//QString version = CWebService::sendCommandRest_Version_Remote_Cloud(miniservers[0], "dev/sys/version", "value");
//QString wholeVersionResponse = CWebService::sendCommandRest_Version_Remote_Cloud(miniservers[0], "dev/sys/version", "");
//QString versionLocal = CWebService::sendCommandRest_Version_Local_Gen1(miniservers[2], "dev/sys/version", "value");
//QString wholeVersionResponseLocal = CWebService::sendCommandRest_Version_Local_Gen1(miniservers[2], "dev/sys/version", "");
//CLoxAppJson loxappRemote = CWebService::sendCommandRest_LoxAppJson_Remote_Cloud(miniservers[2], "data/LoxAPP3.json"); 
//CLoxAppJson loxappLocal = CWebService::sendCommandRest_LoxAppJson_Local_Gen1(miniservers[2], "data/LoxAPP3.json");
//
//QString versionLocalTestkoffer = CWebService::sendCommandRest_Version_Local_Gen1(testkofferZuhause, "dev/sys/version", "value");
//QString wholeVersionResponseTestkoffer = CWebService::sendCommandRest_Version_Local_Gen1(testkofferZuhause, "dev/sys/version", "");
//
//testkofferZuhause.setLocalIP("192.168.178.60");
//QString versionLocalTestkofferIP = CWebService::sendCommandRest_Version_Local_Gen1(testkofferZuhause, "dev/sys/version", "value");
//QString wholeVersionResponseTestkofferIP = CWebService::sendCommandRest_Version_Local_Gen1(testkofferZuhause, "dev/sys/version", "");