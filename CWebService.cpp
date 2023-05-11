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

