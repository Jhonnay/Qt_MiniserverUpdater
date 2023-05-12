#pragma once
#include "stdafx.h"


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
	static void startConfig_Language(QString configPath, QString Language);
	static void killAllConfigs();
	void sendCommand(const QString& ip, int port, const QString& msg);
	void Disconnect();
	void Update();
	void LoadFromMiniserver();
	
};

