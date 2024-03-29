
#include <QList>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>
#include "CMiniserver.h"
#include "CApplicationSettings.h"
#include "MyConstants.h"

class FileParser {
public:
    static QList<CMiniserver> parseMiniserverJsonFile(const QString& fileName) {
        QList<CMiniserver> miniservers;

        try {
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
                QMessageBox::warning(nullptr, "Miniserver Json", QString::fromStdString(MyConstants::Strings::MessageBox_MiniserverList_could_not_parse));
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
                        if (miniserver.getConfigLanguage().empty()) { miniserver.setConfigLanguage("5"); };
                        miniservers.append(miniserver);
                    }
                    else {
                        qWarning() << "Failed to parse JSON data from file:" << fileName;
                        QMessageBox::warning(nullptr, "Miniserver Json", QString::fromStdString(MyConstants::Strings::MessageBox_MiniserverList_could_not_parse));
                    }
                }
            }
        }
        catch (...) {
            qWarning() << "An error occurred while parsing the JSON file:" << fileName;
            QMessageBox::warning(nullptr, "Miniserver Json", QString::fromStdString(MyConstants::Strings::MessageBox_MiniserverList_could_not_parse));
            miniservers.clear();
            return miniservers;
        }
        

        return miniservers;
    }

    static ApplicationSettings parseApplicationSettingsJsonFile(const QString& filePath) {
        ApplicationSettings application = ApplicationSettings();

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // Handle error opening file
            return application;
        }

        try {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
            QJsonObject jsonObj = jsonDoc.object();

            // Read values from JSON object and set them in the CApplication object
            application.setBUseDefaultConfiguration(jsonObj.value("BUseDefaultConfiguration").toBool());
            application.setStrDefaultConfigurationPath(jsonObj.value("StrDefaultConfigurationPath").toString().toStdString());
            application.setBUseDefaultConfig(jsonObj.value("BUseDefaultConfig").toBool());
            application.setStrDefaultConfigPath(jsonObj.value("StrDefaultConfigPath").toString().toStdString());
        }
        catch (const QJsonParseError& error) {
            // Handle JSON parsing error
            qDebug() << "Applcation Settings JSON parsing error:" << error.errorString();
            QMessageBox::warning(nullptr, "Application Settings", QString::fromStdString(MyConstants::Strings::MessageBox_ApplicationSettings_could_not_parse));
        }
        catch (...) {
            // Handle any other exception
            qDebug() << "An error occurred while parsing the JSON file.";
            QMessageBox::warning(nullptr, "Application Settings", QString::fromStdString(MyConstants::Strings::MessageBox_ApplicationSettings_could_not_parse));
            application = ApplicationSettings();
            return application;
        }

        return application;
    }

    static QString checkIfApplicationSettingsExist(QString applicationSettingsFileName) {
        QString roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        //TODO: Make this to function (also usedn in Application Settings dialog) slighty different
        QDir dir(roamingPath);
        if (!dir.exists()) {
            QMessageBox::warning(nullptr, "Application Settings", "Could not open path: " + roamingPath);
            dir.mkpath(roamingPath);
            if (!dir.exists()) {
                QMessageBox::warning(nullptr, "Application Settings", "Could not create Path: " + roamingPath);
            }
            else {
                QMessageBox::information(nullptr, "Application Settings", "Successfully created path: " + roamingPath);
            }

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

    static void saveMiniserverJsonFile(const QList<CMiniserver>& miniservers, const QString& filePath)
    {
        QJsonArray jsonArray;

        // Convert the list of CMiniserver objects to JSON
        foreach(const CMiniserver & miniserver, miniservers)
        {
            QJsonObject jsonObj;
            jsonObj["serialNumer"] = QString::fromStdString(miniserver.getSerialNumber());
            jsonObj["adminUser"] = QString::fromStdString(miniserver.getAdminUser());
            jsonObj["adminPassWord"] = QString::fromStdString(miniserver.getAdminPassword());
            jsonObj["MSVersion"] = QString::fromStdString(miniserver.getMiniserverVersion());
            jsonObj["MSStatus"] = QString::fromStdString(miniserver.getMiniserverStatus());
            jsonObj["UpdateLevel"] = QString::fromStdString(miniserver.getUpdatelevel());
            jsonObj["VersionColor"] = QString::fromStdString(miniserver.getVersionColor());
            jsonObj["MSProject"] = QString::fromStdString(miniserver.getMiniserverProject());
            jsonObj["MSConfiguration"] = QString::fromStdString(miniserver.getMiniserverConfiguration());
            jsonObj["LocalIPAdress"] = QString::fromStdString(miniserver.getLocalIP());
            jsonObj["ConfigLanguage"] = QString::fromStdString(miniserver.getConfigLanguage());

            jsonArray.append(jsonObj);
        }

        // Create the JSON document
        QJsonDocument jsonDoc(jsonArray);

        // Convert the JSON document to byte array
        QByteArray jsonData = jsonDoc.toJson();

        // Save the JSON data to the file
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonData);
            file.close();
        }
        else {
            qWarning() << "Failed to open file for writing:" << filePath;
        }


    }

};

