#pragma once
#include "stdafx.h"
#include <QThread>
#include "CMiniserver.h"
#include "CWebService.h"

class CDownloadProgFolderWorker  : public QThread
{
	Q_OBJECT

public:
	CDownloadProgFolderWorker(QObject *parent);
	~CDownloadProgFolderWorker();
	void setMiniserver(CMiniserver ms);

protected:
	void run() override;

	void openFolderInExplorer(const QString& folderPath);

private:
	CMiniserver miniserver;

signals:
	void updateStatusBarProgress(int progress, QString progresstext);

};
