#pragma once

#include "stdafx.h"
#include "CApplicationSettings.h"
#include <QDialog>


class Qt_ApplicationSettings : public QDialog
{
	Q_OBJECT

public:
	Qt_ApplicationSettings(const QString& title, ApplicationSettings* settings = nullptr, QWidget* parent = nullptr);
	~Qt_ApplicationSettings();

	
	static ApplicationSettings createDialog(const QString& title, ApplicationSettings* settings = nullptr, QWidget* parent = nullptr);
private:
	QLineEdit* lineEditDefaulMiniserverList;
	QLineEdit* lineEditDefaultConfigExe;
	QCheckBox* checkBoxDefaultMiniserverList = new QCheckBox("Use Miniserver List on Startup");
	QCheckBox* checkBoxDefaultConfigExe = new QCheckBox("Use Config Exe on  Startup");;
	ApplicationSettings* m_settings;
	QLabel* label1 = new QLabel("Default Miniserver List:");
	QPushButton* browseButton1 = new QPushButton("Browse");
	QLabel* label2 = new QLabel("Default Config Path:"); 
	QPushButton* browseButton2 = new QPushButton("Browse");
	QDialogButtonBox* buttonBox;
	QVBoxLayout* layout;

	ApplicationSettings getApplicationSettings() const;
};
