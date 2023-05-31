#include "stdafx.h"
#include "CWebService.h"
#include "MyConstants.h"


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QByteArray>
#include <QAuthenticator>
#include "CSerialNumberHyperlinkDelegate.h"



QString CWebService::getCloudDNSLink(CMiniserver miniserver)
{
    QString str_cloudLink = "";

    QString url = QString::fromStdString(MyConstants::Strings::Link_CloudDNS) + QString::fromStdString(miniserver.getSerialNumber());
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");


    // set up the authentication
    QString authString = QString("%1:%2").arg(miniserver.getAdminUser().c_str()).arg(miniserver.getAdminPassword().c_str());
    QByteArray auth = authString.toLocal8Bit().toBase64();
    QString headerData = "Basic " + auth;
    request.setRawHeader("Authorization", headerData.toUtf8());

    // send the request
    QNetworkReply* reply = manager->get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // check for Unauthorized status code
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 401)
        {
            QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            QNetworkRequest redirectRequest(redirectUrl);
            redirectRequest.setRawHeader("Authorization", headerData.toUtf8());
            QNetworkReply* redirectReply = manager->get(redirectRequest);
            QEventLoop redirectLoop;
            QObject::connect(redirectReply, SIGNAL(finished()), &redirectLoop, SLOT(quit()));
            redirectLoop.exec();
            reply = redirectReply;

            if (reply->error() == QNetworkReply::NoError && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
            {
                str_cloudLink = reply->url().toString();
            }
        }
        else
        {
            str_cloudLink = reply->url().toString();
        }
    }

    delete manager;
    
    //delete reply;
    

    return str_cloudLink;
}



QString CWebService::sendCommandRest_Version_Remote_Cloud(CMiniserver miniserver, QString command, QString interestedValue)
{
    QString receivedData = "error";
    QString url = CWebService::getCloudDNSLink(miniserver);
    qDebug() << "Url: " << url;
    if (!url.isEmpty()) {
        url = url + command;
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
        request.setTransferTimeout(3000);

        // set up the authentication
        QString authString = QString("%1:%2").arg(miniserver.getAdminUser().c_str()).arg(miniserver.getAdminPassword().c_str());
        QByteArray auth = authString.toLocal8Bit().toBase64();
        QString headerData = "Basic " + auth;
        request.setRawHeader("Authorization", headerData.toUtf8());

        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        if (reply->error() == QNetworkReply::NoError)
        {
            // check for Unauthorized status code
            if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 401)
            {
                QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
                QNetworkRequest redirectRequest(redirectUrl);
                redirectRequest.setRawHeader("Authorization", headerData.toUtf8());
                QNetworkReply* redirectReply = manager->get(redirectRequest);
                QEventLoop redirectLoop;
                QObject::connect(redirectReply, SIGNAL(finished()), &redirectLoop, SLOT(quit()));
                redirectLoop.exec();
                reply = redirectReply;

                if (reply->error() == QNetworkReply::NoError && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
                {
                    receivedData = reply->readAll();
                }
            }

            QByteArray data = reply->readAll();
            receivedData = data;
            qDebug() << "Data: " << receivedData;
            
            if (interestedValue.isEmpty())
                return receivedData;
            
            receivedData = receivedData.mid(receivedData.indexOf(interestedValue) + interestedValue.length() + 2);
            receivedData = receivedData.left(8);
            int index = receivedData.lastIndexOf('"');
            if (index != -1) {
                receivedData = receivedData.left(index);
            }
        
        }

        delete manager;
        

    }
    qDebug() << "Data: " << receivedData;
    return receivedData;
    
}


QString CWebService::sendCommandRest_Version_Local_Gen1(CMiniserver miniserver, QString command, QString interestedValue)
{
    QString receivedData = "error";
    QString url = "http://" + QString::fromStdString(miniserver.getLocalIP()) + "/" + command;
    qDebug() << "Url: " << url;
    
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
    request.setTransferTimeout(3000);

    // set up the authentication
    QString authString = QString("%1:%2").arg(miniserver.getAdminUser().c_str()).arg(miniserver.getAdminPassword().c_str());
    QByteArray auth = authString.toLocal8Bit().toBase64();
    QString headerData = "Basic " + auth;
    request.setRawHeader("Authorization", headerData.toUtf8());

    QNetworkReply* reply = manager->get(request);
    

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // check for Unauthorized status code
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 401)
        {
            QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            QNetworkRequest redirectRequest(redirectUrl);
            redirectRequest.setRawHeader("Authorization", headerData.toUtf8());
            QNetworkReply* redirectReply = manager->get(redirectRequest);
            QEventLoop redirectLoop;
            QObject::connect(redirectReply, SIGNAL(finished()), &redirectLoop, SLOT(quit()));
            redirectLoop.exec();
            reply = redirectReply;

            if (reply->error() == QNetworkReply::NoError && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
            {
                receivedData = reply->readAll();
            }
        }

        QByteArray data = reply->readAll();
        receivedData = data;
        qDebug() << "Data: " << receivedData;

        if (interestedValue.isEmpty())
            return receivedData;

        receivedData = receivedData.mid(receivedData.indexOf(interestedValue) + interestedValue.length() + 2);
        receivedData = receivedData.left(8);

        int index = receivedData.lastIndexOf('"');
        if (index != -1) {
            receivedData = receivedData.left(index);
        }

        delete manager;
    }
    qDebug() << "Data: " << receivedData;
    return receivedData;
}


CLoxAppJson CWebService::sendCommandRest_LoxAppJson_Remote_Cloud(CMiniserver miniserver, QString command)
{
    QString receivedData = "error";
    QString url = CWebService::getCloudDNSLink(miniserver);
    CLoxAppJson cLoxAppJson;

    if (!url.isEmpty()) {
        url = url + command;
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
        request.setTransferTimeout(3000);

        // set up the authentication
        QString authString = QString("%1:%2").arg(miniserver.getAdminUser().c_str()).arg(miniserver.getAdminPassword().c_str());
        QByteArray auth = authString.toLocal8Bit().toBase64();
        QString headerData = "Basic " + auth;
        request.setRawHeader("Authorization", headerData.toUtf8());

        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        if (reply->error() == QNetworkReply::NoError)
        {
            // check for Unauthorized status code
            if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 401)
            {
                QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
                QNetworkRequest redirectRequest(redirectUrl);
                redirectRequest.setRawHeader("Authorization", headerData.toUtf8());
                QNetworkReply* redirectReply = manager->get(redirectRequest);
                QEventLoop redirectLoop;
                QObject::connect(redirectReply, SIGNAL(finished()), &redirectLoop, SLOT(quit()));
                redirectLoop.exec();
                reply = redirectReply;

                if (reply->error() == QNetworkReply::NoError && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
                {
                    receivedData = reply->readAll();
                }
            }

            try
            {
                QByteArray data = reply->readAll();
                receivedData = data;
                qDebug() << "Data: " << receivedData;
                QJsonDocument jsondata = QJsonDocument::fromJson(receivedData.toUtf8());
                if (jsondata.isObject()) {
                    QJsonObject rootObj = jsondata.object();
                    if (rootObj.contains("msInfo") && rootObj["msInfo"].isObject()) {
                        QJsonObject msInfoObj = rootObj["msInfo"].toObject();
                        QJsonDocument msInfoDoc(msInfoObj);
                        QByteArray msInfoBytes = msInfoDoc.toJson();
                        QString msInfo = QString::fromUtf8(msInfoBytes.constData(), msInfoBytes.size());
                        QJsonDocument msInfoJson = QJsonDocument::fromJson(msInfo.toUtf8());
                        QJsonObject jsonObj = msInfoJson.object();
                            
                        cLoxAppJson = CLoxAppJson(
                            msInfoObj.value("serialNr").toString().toStdString(),
                            msInfoObj.value("msName").toString().toStdString(),
                            msInfoObj.value("projectName").toString().toStdString(),
                            msInfoObj.value("localUrl").toString().toStdString(),
                            msInfoObj.value("remoteUrl").toString().toStdString(),
                            msInfoObj.value("tempUnit").toInt(),
                            msInfoObj.value("currency").toString().toStdString(),
                            msInfoObj.value("squareMeasure").toString().toStdString(),
                            msInfoObj.value("location").toString().toStdString(),
                            msInfoObj.value("latitude").toDouble(),
                            msInfoObj.value("gatewayType").toInt(),
                            msInfoObj.value("longitude").toDouble(),
                            msInfoObj.value("altitude").toInt(),
                            msInfoObj.value("languageCode").toString().toStdString(),
                            msInfoObj.value("heatPeriodStart").toString().toStdString(),
                            msInfoObj.value("heatPeriodEnd").toString().toStdString(),
                            msInfoObj.value("coolPeriodStart").toString().toStdString(),
                            msInfoObj.value("coolPeriodEnd").toString().toStdString(),
                            msInfoObj.value("catTitle").toString().toStdString(),
                            msInfoObj.value("roomTitle").toString().toStdString(),
                            msInfoObj.value("miniserverType").toInt(),
                            msInfoObj.value("deviceMonitor").toString().toStdString(),
                            CurrentUser(
                                msInfoObj.value("currentUser").toObject().value("name").toString().toStdString(),
                                msInfoObj.value("uuid").toObject().value("uuid").toString().toStdString(),
                                msInfoObj.value("isAdmin").toObject().value("isAdmin").toBool(),
                                msInfoObj.value("changePassword").toObject().value("changePassword").toBool(),
                                msInfoObj.value("userRights").toObject().value("userRights").toDouble()
                            )
                        );
                    }
                }

                
            }
            catch (...)
            {
                cLoxAppJson.projectName = MyConstants::Strings::Listview_ProjectName_Invalid_JSON;
            }



        }

        delete manager;


    }
    return cLoxAppJson;

}

CLoxAppJson CWebService::sendCommandRest_LoxAppJson_Local_Gen1(CMiniserver miniserver, QString command)
{
    QString receivedData = "-1213"; //error
    QString url = "http://" + QString::fromStdString(miniserver.getLocalIP()) + "/" + command;
    CLoxAppJson cLoxAppJson;

    
        
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
    request.setTransferTimeout(3000);

    // set up the authentication
    QString authString = QString("%1:%2").arg(miniserver.getAdminUser().c_str()).arg(miniserver.getAdminPassword().c_str());
    QByteArray auth = authString.toLocal8Bit().toBase64();
    QString headerData = "Basic " + auth;
    request.setRawHeader("Authorization", headerData.toUtf8());

    QNetworkReply* reply = manager->get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // check for Unauthorized status code
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 401)
        {
            QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            QNetworkRequest redirectRequest(redirectUrl);
            redirectRequest.setRawHeader("Authorization", headerData.toUtf8());
            QNetworkReply* redirectReply = manager->get(redirectRequest);
            QEventLoop redirectLoop;
            QObject::connect(redirectReply, SIGNAL(finished()), &redirectLoop, SLOT(quit()));
            redirectLoop.exec();
            reply = redirectReply;

            if (reply->error() == QNetworkReply::NoError && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
            {
                receivedData = reply->readAll();
            }
        }

        try
        {
            QByteArray data = reply->readAll();
            receivedData = data;
            qDebug() << "Data: " << receivedData;
            QJsonDocument jsondata = QJsonDocument::fromJson(receivedData.toUtf8());
            if (jsondata.isObject()) {
                QJsonObject rootObj = jsondata.object();
                if (rootObj.contains("msInfo") && rootObj["msInfo"].isObject()) {
                    QJsonObject msInfoObj = rootObj["msInfo"].toObject();
                    QJsonDocument msInfoDoc(msInfoObj);
                    QByteArray msInfoBytes = msInfoDoc.toJson();
                    QString msInfo = QString::fromUtf8(msInfoBytes.constData(), msInfoBytes.size());
                    QJsonDocument msInfoJson = QJsonDocument::fromJson(msInfo.toUtf8());
                    QJsonObject jsonObj = msInfoJson.object();

                    cLoxAppJson = CLoxAppJson(
                        msInfoObj.value("serialNr").toString().toStdString(),
                        msInfoObj.value("msName").toString().toStdString(),
                        msInfoObj.value("projectName").toString().toStdString(),
                        msInfoObj.value("localUrl").toString().toStdString(),
                        msInfoObj.value("remoteUrl").toString().toStdString(),
                        msInfoObj.value("tempUnit").toInt(),
                        msInfoObj.value("currency").toString().toStdString(),
                        msInfoObj.value("squareMeasure").toString().toStdString(),
                        msInfoObj.value("location").toString().toStdString(),
                        msInfoObj.value("latitude").toDouble(),
                        msInfoObj.value("gatewayType").toInt(),
                        msInfoObj.value("longitude").toDouble(),
                        msInfoObj.value("altitude").toInt(),
                        msInfoObj.value("languageCode").toString().toStdString(),
                        msInfoObj.value("heatPeriodStart").toString().toStdString(),
                        msInfoObj.value("heatPeriodEnd").toString().toStdString(),
                        msInfoObj.value("coolPeriodStart").toString().toStdString(),
                        msInfoObj.value("coolPeriodEnd").toString().toStdString(),
                        msInfoObj.value("catTitle").toString().toStdString(),
                        msInfoObj.value("roomTitle").toString().toStdString(),
                        msInfoObj.value("miniserverType").toInt(),
                        msInfoObj.value("deviceMonitor").toString().toStdString(),
                        CurrentUser(
                            msInfoObj.value("currentUser").toObject().value("name").toString().toStdString(),
                            msInfoObj.value("uuid").toObject().value("uuid").toString().toStdString(),
                            msInfoObj.value("isAdmin").toObject().value("isAdmin").toBool(),
                            msInfoObj.value("changePassword").toObject().value("changePassword").toBool(),
                            msInfoObj.value("userRights").toObject().value("userRights").toDouble()
                        )
                    );
                }
            }


        }
        catch (...)
        {
            cLoxAppJson.projectName = MyConstants::Strings::Listview_ProjectName_Invalid_JSON;
        }



    }

    delete manager;


    
    return cLoxAppJson;

}

int CWebService::DownloadProgFolder(CMiniserver miniserver)
{

    QString proglistRAW;
    QString link;

    //local
    if (!miniserver.getLocalIP().empty()) {
        proglistRAW = sendCommandRest_Version_Local_Gen1(miniserver, "dev/fslist/prog", "");
        link = CSerialNumberHyperlinkDelegate::generateLink(QString::fromStdString(miniserver.getSerialNumber()), QString::fromStdString(miniserver.getLocalIP()));
    }
    else { //remote
        proglistRAW = sendCommandRest_Version_Remote_Cloud(miniserver, "dev/fslist/prog", "");
        link = getCloudDNSLink(miniserver);
    }

    QList<QString> files = GetFileNamesOfProgList(proglistRAW);
    QString path = "/dev/fsget/prog/";
    QString adminUser = QString::fromStdString(miniserver.getAdminUser());
    QString adminpassword = QString::fromStdString(miniserver.getAdminPassword());
    link += path;
    int i = 1;
    
    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + QDir::separator() + "ProgFolder" + QDir::separator();
    QNetworkAccessManager networkManager;


    for (const QString& file : files)
    {

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

                    i++;
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

    }
    networkManager.deleteLater();

    return 1;
}

QList<QString> CWebService::GetFileNamesOfProgList(QString& progList)
{
    QStringList lines = progList.split(QRegularExpression("[\r\n]"), Qt::SkipEmptyParts);
    QList<QString> validFiles;
    for (int i = 2; i < lines.size(); i++)
    {
        QString fileName = ExtractFileName(lines[i]);
        if (!fileName.isEmpty())
        {
            validFiles.append(fileName);
        }
    }
    return validFiles;
}


QString CWebService::ExtractFileName(QString& inputString)
{
    QStringList parts = inputString.split(' ');

    for (int i = parts.size() - 1; i >= 0; i--)
    {
        QString part = parts[i];
        if (part.endsWith(".zip") || part.endsWith(".json") || part.endsWith("Loxone")
            || part.endsWith("LoxCC") || part.endsWith("bin"))
        {
            return part;
        }
    }

    return QString();
}
