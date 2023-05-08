#include "stdafx.h"
#include "CConfig.h"
#include <QtGui>


const QList<QString> CConfig::LanguageList = {
    "CAT", "CHS", "CSY", "DEU", "ENG", "ENU", "ESN", "FRA", "ITA", "HUN", "NLD", "NOR", "PLK", "ROM", "RUS", "SKY", "TRK"
};


void CConfig::startConfig(QString configPath)
{
    QProcess* p = new QProcess();
    p->start(configPath);

    if (!p->waitForStarted()) {
        QMessageBox::warning(nullptr, "Error", "Failed to start process");
        return;
    }
}

void CConfig::startConfig_Language(QString configPath, QString Language)
{
    try
    {
        if (!Language.isNull() && !Language.isEmpty())
        {
            QProcess* p = new QProcess();
            QStringList arguments;
            arguments << "/Language=" + CConfig::LanguageList.at(Language.toInt());
            p->start(configPath, arguments);

            if (!p->waitForStarted()) {
                QMessageBox::warning(nullptr, "Error", "Failed to start process");
                return;
            }
        }
        else
        {
            CConfig::startConfig(configPath);
        }
    }
    catch (std::exception& ex)
    {
        QMessageBox::critical(nullptr, "Error", ex.what());
    }
}

void CConfig::killAllConfigs() {
    //system("taskkill /im LoxoneConfig.exe /f");
    //QProcess::startDetached("taskkill /im LoxoneConfig.exe /f");
    QProcess process;
    process.start("taskkill", QStringList() << "/im" << "LoxoneConfig.exe" << "/f");
    process.waitForFinished();

}