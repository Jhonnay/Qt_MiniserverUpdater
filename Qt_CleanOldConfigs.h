#pragma once

#include "stdafx.h"
#include <QDialog>

class Qt_CleanOldConfigs : public QDialog
{
	Q_OBJECT

public:
	Qt_CleanOldConfigs(QString title, QString pathPrograms, QString pathAppdata, QWidget *parent = nullptr);
	void InitializeDialogWithSizes(QString& pathPrograms, QString& pathProgramData);
	~Qt_CleanOldConfigs();

	void compareDirectories(const QString& dirPath1, const QString& dirPath2);

	void displayAllFolders(const QString& dirPath1, QStringList& folders1, const QString& dirPath2, QStringList& folders2);

	void reduceDirectoryFilenamesToVersionString(const QString& dirPath1, QStringList& folders1);

	uintmax_t getDirectorySize(const std::filesystem::path& dir);

	uintmax_t calculateDirectorySizeOfQListWidget(QString dir);

	QString formatSize(uintmax_t sizeInBytes);

	QString reduceFileNameToVersionString(QString filename);
	bool isRunningAsAdmin();
	void performCleaning();

	bool cleanFolder(QString path);

public slots:
	void addSelectedFoldersToCleanUP();

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
	QMenu* contextMenuListPrograms;
	QAction* actionContextMenuListPrograms; 
	QPushButton* clean;

	uintmax_t sizeCleanPrograms = 0; 
	uintmax_t sizeCleanProgramData = 0; 



};
