#pragma once

#include "stdafx.h"
#include <QDialog>

class Qt_CleanOldConfigs : public QDialog
{
	Q_OBJECT

public:
	Qt_CleanOldConfigs(QString title, QString pathPrograms, QString pathAppdata, QWidget *parent = nullptr);
	~Qt_CleanOldConfigs();

	void compareDirectories(const QString& dirPath1, const QString& dirPath2);

	uintmax_t getDirectorySize(const std::filesystem::path& dir);

	uintmax_t calculateCleanSize(QString dir);

	QString formatSize(uintmax_t sizeInBytes);

private:
	QHBoxLayout* hboxScroll;
	QHBoxLayout* hboxLabels;
	QHBoxLayout* hboxSizes;
	QVBoxLayout* vbox; 
	QPushButton* ok;
	QLabel* pathProgramData;
	QLabel* pathPrograms;
	QLabel* sizeProgramData;
	QLabel* sizePrograms;
	QListWidget* listWidgetProgramData;
	QListWidget* listWidgetPrograms;
	QDialogButtonBox* buttonBox;



};
