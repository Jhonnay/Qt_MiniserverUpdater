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

QList<CMiniserver> parseMiniserverJsonFile(const QString& fileName);
ApplicationSettings parseApplicationSettingsJsonFile(const QString& filePath);
QString checkIfApplicationSettingsExist(QString applicationSettingsFileName);

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    std::cout << "Hello, World!" << std::endl;
    
    QList<CMiniserver> miniservers;
    //MainWindow
    Qt_MiniserverUpdater* mainwindow = new Qt_MiniserverUpdater(&miniservers, nullptr);
    QString applicationSettingsPath = checkIfApplicationSettingsExist("ApplicationSettings.json");
    ApplicationSettings applicationSettings;
    if (!applicationSettingsPath.isEmpty()) {
        applicationSettings = parseApplicationSettingsJsonFile(applicationSettingsPath);
        mainwindow->setApplicationsettings(&applicationSettings);
        if (applicationSettings.getBUseDefaultConfig()) {
            mainwindow->setConfigEXEPath(QString::fromStdString(applicationSettings.getStrDefaultConfigPath()));
        }
        if (applicationSettings.getBUseDefaultConfiguration()) {
            miniservers = parseMiniserverJsonFile(QString::fromStdString(applicationSettings.getStrDefaultConfigurationPath()));
            mainwindow->setMiniserverList(&miniservers);
        }
    }

    //mainWindow.setCentralWidget(centralWidget);
    mainwindow->setWindowTitle("Miniserver Updater (c) Musat Version 0.1");
    mainwindow->resize(1000, 600);
    mainwindow->show();




    return a.exec();
}




QList<CMiniserver> parseMiniserverJsonFile(const QString& fileName)
{
    QList<CMiniserver> miniservers;

    // Open the JSON file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << fileName;
        return miniservers;
    }

    // Read the JSON data from the file
    QByteArray jsonData = file.readAll();
    file.close();

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse JSON data from file:" << fileName;
        return miniservers;
    }

    // Convert the JSON data to a list of CMiniserver objects
    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();
        foreach(const QJsonValue & jsonValue, jsonArray) {
            if (jsonValue.isObject()) {
                QJsonObject jsonObj = jsonValue.toObject();
                CMiniserver miniserver(
                    jsonObj["serialNumer"].toString().toStdString(),
                    jsonObj["adminUser"].toString().toStdString(),
                    jsonObj["adminPassWord"].toString().toStdString(),
                    jsonObj["MSVersion"].toString().toStdString(),
                    jsonObj["MSStatus"].toString().toStdString(),
                    jsonObj["UpdateLevel"].toString().toStdString(),
                    jsonObj["VersionColor"].toString().toStdString(),
                    jsonObj["MSProject"].toString().toStdString(),
                    jsonObj["MSConfiguration"].toString().toStdString(),
                    jsonObj["LocalIPAdress"].toString().toStdString(),
                    jsonObj["ConfigLanguage"].toString().toStdString()
                );
                miniservers.append(miniserver);
            }
        }
    }

    return miniservers;
}

ApplicationSettings parseApplicationSettingsJsonFile(const QString& filePath)
{
    ApplicationSettings application = ApplicationSettings();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Handle error opening file
        return application;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    QJsonObject jsonObj = jsonDoc.object();

    // Read values from JSON object and set them in the CApplication object
    application.setBUseDefaultConfiguration(jsonObj.value("BUseDefaultConfiguration").toBool());
    application.setStrDefaultConfigurationPath(jsonObj.value("StrDefaultConfigurationPath").toString().toStdString());
    application.setBUseDefaultConfig(jsonObj.value("BUseDefaultConfig").toBool());
    application.setStrDefaultConfigPath(jsonObj.value("StrDefaultConfigPath").toString().toStdString());

    return application;
}


QString checkIfApplicationSettingsExist(QString applicationSettingsFileName) {
    QString roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    
    QDir dir(roamingPath);
    if (!dir.exists()) {
        //dir.mkpath(".");
        return "";
    }
    

    QString filePath = roamingPath + "/" + applicationSettingsFileName;
    QFile file(filePath);
    if (file.exists()) {
        return filePath;
    }
    else {
        return "";
    }
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