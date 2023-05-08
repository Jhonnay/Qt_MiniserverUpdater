#include "stdafx.h"
#include "Qt_Miniserver_Listview.h"
#include <QtWidgets/QApplication>
#include <stdio.h>
#include <iostream>
#include "CMiniserver.h"
#include "Qt_Bottom_Action_Buttons.h"
#include "Qt_Menubar.h"
#include "Qt_Statusbar.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include "CApplicationSettings.h"

QList<CMiniserver> parseMiniserverJsonFile(const QString& fileName);
ApplicationSettings parseApplicationSettingsJsonFile(const QString& filePath);


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    std::cout << "Hello, World!" << std::endl;

    std::vector <CMiniserver> miniserverlist;
    QList<CMiniserver> miniservers = parseMiniserverJsonFile("C:\\Users\\musatbe\\AppData\\Roaming\\WPF_MiniserverUpdater\\configuration Firma.json");
    ApplicationSettings applicationSettings = parseApplicationSettingsJsonFile("C:\\Users\\musatbe\\AppData\\Roaming\\WPF_MiniserverUpdater\\ApplicationSettings.json");

    for (const auto& miniserver : miniservers) {
        miniserverlist.push_back(miniserver);
    }

    Qt_Bottom_Action_Buttons* bottom_buttons = new Qt_Bottom_Action_Buttons();
    Qt_Menubar* menubar = new Qt_Menubar();
    Qt_Statusbar* statusbar = new Qt_Statusbar();
    Qt_Miniserver_Listview* treeViewMiniserver = new Qt_Miniserver_Listview();

    if (applicationSettings.getBUseDefaultConfig()) {
        statusbar->setConfigExePath(QString::fromStdString(applicationSettings.getStrDefaultConfigPath()));
    }
    if (applicationSettings.getBUseDefaultConfiguration()) {
        treeViewMiniserver->setData(miniserverlist);
    }




    QVBoxLayout* vBox = new QVBoxLayout();
    vBox->addWidget(menubar);
    vBox->addWidget(treeViewMiniserver);
    vBox->addWidget(bottom_buttons);
    vBox->addWidget(statusbar);
    vBox->setSpacing(0);
    vBox->setContentsMargins(0, 0, 0, 0);
    //vBox->insertStretch(-1, 1);

    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(vBox);

    QMainWindow mainWindow;
    mainWindow.setCentralWidget(centralWidget);
    mainWindow.setWindowTitle("Miniserver Updater (c) Musat Version 0.1");
    mainWindow.resize(1000, 600);
    mainWindow.show();


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




