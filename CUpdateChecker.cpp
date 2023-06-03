#include "CUpdateChecker.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QCoreApplication>
#include <qstandardpaths.h>
#include <qversionnumber.h>
#include "MyConstants.h"



CUpdateChecker::CUpdateChecker(QObject *parent)
	: QObject(parent)
{
    // Initialize member variables
    UpdateCheck_with_Internet = false;
    stringApplicationVersion = "0.0.0"; // Replace with your application version
    url_github_Latest = "https://api.github.com/repos/Jhonnay/Qt_MiniserverUpdater/releases/latest"; // Replace with your GitHub repository URL
    token = ""; // Replace with your personal access token if required
}

CUpdateChecker::~CUpdateChecker()
{}


void CUpdateChecker::setApplicationVersion(QString version) {
    stringApplicationVersion = version;
}

void CUpdateChecker::CheckUpdate_and_install_if_user_wants(bool bShow_No_Update_available)
{
    bool updatesAvailable = check_Update_needed();
    if (updatesAvailable)
    {
        QString message = "There is a newer version available! Do you want to download and install?\nCurrent Version: " + stringApplicationVersion + "\nAvailable Version: " + UpdateVersion;
        QMessageBox::StandardButton result = QMessageBox::question(nullptr,"Install Update?", message, QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/MiniserverUpdater.exe";
            Download_and_Install_Latest_Alpha(url_github_Latest, downloadPath, token);
        }
    }
    else if (bShow_No_Update_available)
    {
        if (UpdateCheck_with_Internet)
        {
            QMessageBox::information(nullptr, "Information", QString::fromStdString(MyConstants::Strings::MessageBox_CheckUpdate_no_Update_available));
        }
        else
        {
            QMessageBox::information(nullptr, "Information", QString::fromStdString(MyConstants::Strings::MessageBox_CheckUpdate_no_Internet));
        }
    }
}

void CUpdateChecker::Download_and_Install_Latest_Alpha(const QString& url, const QString& downloadPath, const QString& personalAccessToken)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (compatible; AcmeInc/1.0)");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    if (!personalAccessToken.isEmpty())
    {
        QByteArray authorizationHeader = "Bearer " + personalAccessToken.toUtf8();
        request.setRawHeader("Authorization", authorizationHeader);
    }

    QNetworkAccessManager manager;
    QEventLoop loop;
    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QNetworkReply* reply = manager.get(request);
    loop.exec();

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonObject release = QJsonDocument::fromJson(responseData).object();
    QJsonArray assets = release["assets"].toArray();
    QJsonObject asset = assets[0].toObject();
    QString downloadUrl = asset["browser_download_url"].toString();

    QNetworkRequest downloadRequest(downloadUrl);
    downloadRequest.setRawHeader("Authorization", "Bearer " + personalAccessToken.toUtf8());

    QNetworkReply* downloadReply = manager.get(downloadRequest);
    loop.exec();

    QByteArray downloadData = downloadReply->readAll();
    downloadReply->deleteLater();

    QFile file(downloadPath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(downloadData);
        file.close();

        QProcess::startDetached(downloadPath);
    }
    else
    {
        qDebug() << "Error opening file for writing: " << file.errorString();
    }
}

bool CUpdateChecker::check_Update_needed()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url_github_Latest);
    UpdateCheck_with_Internet = false;

    if (!token.isEmpty())
    {
        QByteArray authData = "Bearer " + token.toUtf8();
        request.setRawHeader("Authorization", authData);
    }

    QEventLoop loop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkReply* reply = manager->get(request);

    loop.exec();
    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    if (!responseData.isEmpty()) {
        UpdateCheck_with_Internet = true;
    }
    QJsonObject release = QJsonDocument::fromJson(responseData).object();
    

    QString latestVersion = release["tag_name"].toString();
    latestVersion.remove("v");
    latestVersion.remove(latestVersion.indexOf("-"), latestVersion.length());
    QVersionNumber currentVersion = QVersionNumber::fromString(stringApplicationVersion);
    QVersionNumber latestVersionObj = QVersionNumber::fromString(latestVersion);
    delete manager;
    if (latestVersionObj > currentVersion)
    {
        UpdateVersion = latestVersion;
        return true;
    }

    return false;
        

    
}

QString CUpdateChecker::getDownloadPath() const
{
    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    return QDir::toNativeSeparators(downloadPath);
}