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

QString getCloudDNSLink(CMiniserver miniserver)
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
    delete reply;

    return str_cloudLink;
}



