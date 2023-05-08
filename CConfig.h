#pragma once


class CConfig
{
public:
	static const QList<QString> LanguageList;
	static void startConfig(QString configPath);
	static void startConfig_Language(QString configPath, QString Language);
	static void killAllConfigs();
};

