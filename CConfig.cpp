#include "stdafx.h"
#include "CConfig.h"
#include <QtGui>
#include <QUdpSocket>

#include <QFile>


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

void CConfig::startConfig(QString configPath, QString projectPath)
{
    try
    {
        QProcess* p = new QProcess();
        QStringList arguments;
        arguments << "/auto" << projectPath;
        p->start(configPath, arguments);
    }
    catch (const std::exception& ex)
    {
        QMessageBox::critical(nullptr, "Error", ex.what());
    }
}

void CConfig::startConfig_Language(QString configPath, QString projectPath, QString language)
{
    try
    {
        QString lang = (language.isNull()) ? "ENU" : CConfig::LanguageList.at(language.toInt());
        QProcess* p = new QProcess();
        QStringList arguments;
        arguments << "/auto" << projectPath << "/Language=" + lang;
        p->start(configPath, arguments);
    }
    catch (const std::exception& ex)
    {
        QMessageBox::critical(nullptr, "Error", ex.what());
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

void CConfig::sendCommand(const QString& ip, int port, const QString& msg)
{
    QUdpSocket udpSocket;
    QHostAddress addr(ip);

    try
    {
        udpSocket.connectToHost(addr, port);
        QByteArray sendBytes = msg.toUtf8();

        if (udpSocket.write(sendBytes) == sendBytes.size())
        {
            udpSocket.close();
            qDebug() << "Command : '" << msg << "' was successfully sent";
        }
        else
        {
            qDebug() << "Error sending command to Config.";
        }
    }
    catch (const std::exception& ex)
    {
        QThread::sleep(1);
        qDebug() << ex.what();
        QMessageBox::critical(nullptr, "Debug", QString("Debug:\n%1\nTrying again.").arg(ex.what()));
        sendCommand(ip, port, msg); // Recursive call to retry sending command
    }
}

void CConfig::Disconnect()
{
    sendCommand("127.0.0.1", 7770, "D");
}

void CConfig::Update()
{
    sendCommand("127.0.0.1", 7770, "U");
}

void CConfig::LoadFromMiniserver()
{
    sendCommand("127.0.0.1", 7770, "L");
}

void CConfig::closeConfig()
{
   
    qDebug()  << " Config.exe<- Task!";
    p.kill();
    qDebug() << "Task killed!";
}

QString CConfig::getConfigFileVersion(const QString& fileName) {
    QFile file(fileName);
    if (!file.exists()) return QString();
   
    DWORD handle;
    DWORD size = GetFileVersionInfoSize(file.fileName().toStdWString().c_str(), &handle);
    if (size == 0) return QString();
   
    QByteArray data(size, 0);
    if (!GetFileVersionInfo(file.fileName().toStdWString().c_str(), handle, size, data.data())) return QString();
   
    VS_FIXEDFILEINFO* info = nullptr;
    UINT len;
    if (!VerQueryValue(data.constData(), L"\\", reinterpret_cast<void**>(&info), &len)) return QString();
   
    return QString("%1.%2.%3.%4")
        .arg(HIWORD(info->dwFileVersionMS))
        .arg(LOWORD(info->dwFileVersionMS))
        .arg(HIWORD(info->dwFileVersionLS))
        .arg(LOWORD(info->dwFileVersionLS));
    
}