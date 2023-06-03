#pragma once

#include <QObject>

#include "stdafx.h"
#include <QString>
#include <QMessageBox>
#include <QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QProcess>
#include <QUrl>


class CUpdateChecker  : public QObject
{
	Q_OBJECT

public:
	CUpdateChecker(QObject *parent);
	~CUpdateChecker();

    void setApplicationVersion(QString version);
    void CheckUpdate_and_install_if_user_wants(bool bShow_No_Update_available);
    void Download_and_Install_Latest_Alpha(const QString& url, const QString& downloadPath, const QString& personalAccessToken);
    bool check_Update_needed();


private:
    bool UpdateCheck_with_Internet;
    QString stringApplicationVersion;
    QString UpdateVersion;
    QString url_github_Latest;
    QString token;
    QString getDownloadPath() const;

};
