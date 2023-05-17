#pragma once
#include "stdafx.h"
#include <windows.h>
#pragma comment (lib, "version.lib")


class CConfig
{
public:
	static std::string miniserver_ip;
	static std::string user;
	static std::string pw;
	static int port_receive;
	static int port_send;
	QProcess p;
	static const QList<QString> LanguageList;
	static void startConfig(QString configPath);
	void startConfig(QString configPath, QString projectPath);
	void startConfig_Language(QString configPath, QString projectPath, QString language);
	static void startConfig_Language(QString configPath, QString Language);
	static void killAllConfigs();
	void sendCommand(const QString& ip, int port, const QString& msg);
	void Disconnect();
	void Update();
	void LoadFromMiniserver();

	void closeConfig();

	static QString getConfigFileVersionFormated(const QString& configPath);

	static QString getConfigFileVersionUnformated(const QString& configPath);
	
};

