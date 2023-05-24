
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

class FileParser {
public:
    static QList<CMiniserver> parseMiniserverJsonFile(const QString& fileName) {
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

    static ApplicationSettings parseApplicationSettingsJsonFile(const QString& filePath) {
        ApplicationSettings application = ApplicationSettings();

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
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

    static QString checkIfApplicationSettingsExist(QString applicationSettingsFileName) {
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

