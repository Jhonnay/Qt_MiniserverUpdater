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
#include "MyApplication.h"
#include <QDir>
#include <QStringList>

#define VERSION "0.3.1"


int main(int argc, char* argv[])
{
    MyApplication app(argc, argv);

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
        mainwindow->setStatusbarProgress(100, "Settings loaded / " + QString::number(miniservers.count()) + " Miniservers loaded");
    }
    else {
        applicationSettings =  ApplicationSettings();
        mainwindow->setApplicationsettings(&applicationSettings);
        mainwindow->setStatusbarProgress(100, "Settings NOT loaded / " + QString::number(miniservers.count()) + " Miniservers loaded");
    }

    mainwindow->checkVersionOnStartup();
    //mainWindow.setCentralWidget(centralWidget);
    mainwindow->setWindowTitle("Miniserver Updater (c) Musat Version " + QString::fromStdString(VERSION));
    mainwindow->resize(1000, 600);
    mainwindow->show();

    return app.exec();
}




