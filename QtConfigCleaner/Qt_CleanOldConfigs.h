#pragma once

#include "../stdafx.h"
#include <QDialog>


class Qt_CleanOldConfigs : public QDialog
{
	Q_OBJECT

public:
	Qt_CleanOldConfigs(QString title, QString pathPrograms, QString pathAppdata, QWidget *parent = nullptr);
	void InitializeDialogWithSizes(QString& pathPrograms, QString& pathProgramData);
	~Qt_CleanOldConfigs();

	void compareDirectories(const QString& dirPath1, const QString& dirPath2);

	bool isVersionLessThan(const QString& version, const QString& referenceVersion);

	void displayAllFolders(const QString& dirPath1, QStringList& folders1, const QString& dirPath2, QStringList& folders2);

	void reduceDirectoryFilenamesToVersionString(const QString& dirPath1, QStringList& folders1);

	uintmax_t getDirectorySize(const std::filesystem::path& dir);

	uintmax_t calculateDirectorySizeOfQListWidget(QString dir);

	QString formatSize(uintmax_t sizeInBytes);

	QString reduceFileNameToVersionString(QString filename);
	bool isRunningAsAdmin();
	void performCleaning();
	int GetToDeleteItemsCount();

	void startCleaning();
	void stopCleaning();
	void updateProgress();

	bool cleanFolder(QString path);

	void reCalculateSizes();

	void prohibitCleaningPrograms();

	void prohibitCleaningProgramData();

signals:
	void cleaningFinished();

public slots:
	void addSelectedProgramsFoldersToCleanUP();
	void addSelectedProgramDataFoldersToCleanUP();


private:
	QHBoxLayout* hboxScroll;
	QHBoxLayout* hboxLabels;
	QHBoxLayout* hboxSizes;
	QVBoxLayout* vbox; 
	QPushButton* ok;
	QPushButton* btn_PathProgramData;
	QPushButton* btn_PathPrograms;
	QLabel* sizeProgramData;
	QLabel* sizePrograms;
	QListWidget* listWidgetProgramData;
	QListWidget* listWidgetPrograms;
	QDialogButtonBox* buttonBox;
	QMenu* contextMenuListPrograms;
	QMenu* contextMenuListProgramData;
	QAction* actionListProgramsAddSelected; 
	QAction* actionListProgramsCleaningProhibited;
	QAction* actionListProgramDataCleaningProhibited;
	QAction* actionListProgramDataAddSelected;
	QPushButton* btn_Clean;
	QPushButton* btn_RefreshSizes;
	QProgressDialog* progressDialog;

	uintmax_t sizeCleanPrograms = 0; 
	uintmax_t sizeCleanProgramData = 0; 

	BOOL m_bStopCleaning;
	int m_nDeleteCount;
	int m_nProgress;



};
