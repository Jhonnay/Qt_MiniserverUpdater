#include "CDownloadProgFolderWorker.h"
#include "CSerialNumberHyperlinkDelegate.h"
#include <QNetworkReply>
#include <QAuthenticator>


CDownloadProgFolderWorker::CDownloadProgFolderWorker(QObject *parent)
	: QThread(parent)
{}

CDownloadProgFolderWorker::~CDownloadProgFolderWorker()
{}

void CDownloadProgFolderWorker::setMiniserver(CMiniserver ms)
{
    this->miniserver = ms;
}

void CDownloadProgFolderWorker::run()
{
    QString progresstext;
    QString proglistRAW;
    QString link;
    int count, progress;

    progresstext = QStringLiteral("⬇ Downloading Prog folder of %1").arg(QString::fromStdString(miniserver.getSerialNumber()));
    emit updateStatusBarProgress(1, progresstext);

    //local
    if (!miniserver.getLocalIP().empty()) {
        proglistRAW = CWebService::sendCommandRest_Version_Local_Gen1(miniserver, "dev/fslist/prog", "");
        link = CSerialNumberHyperlinkDelegate::generateLink(QString::fromStdString(miniserver.getSerialNumber()), QString::fromStdString(miniserver.getLocalIP()));
    }
    else { //remote
        proglistRAW = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "dev/fslist/prog", "","");
        link = CWebService::getCloudDNSLink(miniserver);
    }

    QList<QString> files = CWebService::GetFileNamesOfProgList(proglistRAW);
    QString path = "/dev/fsget/prog/";
    QString adminUser = QString::fromStdString(miniserver.getAdminUser());
    QString adminpassword = QString::fromStdString(miniserver.getAdminPassword());
    link += path;
    progress = 1;
    int progressInt = 1;
    int successes = 0;
    

    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + QDir::separator() + "ProgFolder" + QDir::separator();
    QNetworkAccessManager networkManager;

    count = files.count();
    progressInt = (progress * 100 / 2) / count;

    for (const QString& file : files)
    {
        progresstext = QStringLiteral("%1 %2/%3 ⏳").arg(file).arg(progress).arg(count);
        
        emit updateStatusBarProgress(progressInt, progresstext);

        QNetworkRequest request(link + file);
        request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(adminUser).arg(adminpassword).toUtf8()).toBase64());

        QObject::connect(&networkManager, &QNetworkAccessManager::authenticationRequired, [](QNetworkReply* reply, QAuthenticator* authenticator) {
            authenticator->setUser(reply->property("username").toString());
            authenticator->setPassword(reply->property("password").toString());
            });

        if (!file.isEmpty())
        {
            QCoreApplication::postEvent(qApp, new QEvent(QEvent::User));  // Update UI elements on main UI thread

            QNetworkReply* reply = networkManager.get(request);
            QCoreApplication::processEvents();

            while (!reply->isFinished())
            {
                QCoreApplication::processEvents();
            }

            if (reply->error() == QNetworkReply::NoError)
            {
                QByteArray fileData = reply->readAll();

                if (!QDir(downloadPath + file).exists())
                {
                    QDir().mkpath(downloadPath);
                }

                QString filePath = downloadPath + file;
                QFile file(filePath);
                if (file.open(QIODevice::WriteOnly))
                {
                    file.write(fileData);
                    file.close();

                    QCoreApplication::postEvent(qApp, new QEvent(QEvent::User));  // Update UI elements on main UI thread
                    QCoreApplication::processEvents();

                    successes++;
                    QCoreApplication::postEvent(qApp, new QEvent(QEvent::User));  // Update UI elements on main UI thread
                    QCoreApplication::processEvents();
                }
                else
                {
                    QMessageBox::critical(nullptr, "Error", "Failed to create file: " + filePath);
                }
            }
            else
            {
                QMessageBox::critical(nullptr, "Error", "Failed to download file: " + file + "\n" + reply->errorString());
            }

            reply->deleteLater();
        }
        progressInt = (progress * 100) / count;
        progress++;

    }
    networkManager.deleteLater();
    if (successes > 0) {
        progresstext = QStringLiteral("Downloaded %1/%2! 💪").arg(successes).arg(count);
        emit updateStatusBarProgress(100, progresstext);
    }
    else {
        progresstext = QStringLiteral("Miniserver not reachable! ⛔");
        emit updateStatusBarProgress(100, progresstext);
    }
    

    openFolderInExplorer(downloadPath);
}

void CDownloadProgFolderWorker::openFolderInExplorer(const QString& folderPath)
{
    QUrl url = QUrl::fromLocalFile(folderPath);
    QDesktopServices::openUrl(url);
}
