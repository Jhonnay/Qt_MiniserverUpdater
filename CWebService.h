#pragma once
#include "CMiniserver.h"
#include "CLoxAppJson.h"

class CWebService
{

public:
	static QString getCloudDNSLink(CMiniserver ms);
	static QString sendCommandRest_Version_Remote_Cloud(CMiniserver miniserver, QString command, QString interestedValue);

	static QString sendCommandRest_Version_Local_Gen1(CMiniserver miniserver, QString command, QString interestedValue);

	static CLoxAppJson sendCommandRest_LoxAppJson_Remote_Cloud(CMiniserver miniserver, QString command);

	static CLoxAppJson sendCommandRest_LoxAppJson_Local_Gen1(CMiniserver miniserver, QString command);

	static int DownloadProgFolder(CMiniserver miniserver);
private:
	static QList<QString> GetFileNamesOfProgList(QString& progList);
	static QString ExtractFileName(QString& inputString);
};

