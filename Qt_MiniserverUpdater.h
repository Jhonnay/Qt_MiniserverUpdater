#pragma once
#include "stdafx.h"
#include <QtWidgets/QMainWindow>
#include "ui_Qt_MiniserverUpdater.h"
#include "Qt_Menubar.h"
#include "Qt_Bottom_Action_Buttons.h"
#include "Qt_MiniserverTableView.h"
#include "CApplicationSettings.h"
#include "Qt_Statusbar.h"
#include "CConfig.h"
#include "CUpdateWorker.h"
#include "CRefreshWorker.h"
#include "CConnectConfigWorker.h"
#include "CDownloadProgFolderWorker.h"
#include "CUpdateChecker.h"

class Qt_MiniserverUpdater : public QMainWindow
{
    Q_OBJECT

public:
    Qt_MiniserverUpdater(QWidget* parent = nullptr);
    Qt_MiniserverUpdater(QList<CMiniserver>* miniserverList, QWidget* parent = nullptr);
    ~Qt_MiniserverUpdater();
    
    void setMiniserverList(QList<CMiniserver>* list);
    void updateMiniserverList(QList<CMiniserver>* list); 
    void setConfigEXEPath(QString path);
    void setApplicationsettings(ApplicationSettings* settings);
    void setApplicationVersion(QString version);
    void setStatusbarProgress(int progress, QString progresstext);
    void checkVersionOnStartup();
    
    

public slots:
    void onConnectConfigClicked(const QModelIndex& index, const CMiniserver& miniserver);
    void onRefreshClicked();
    void onConnectConfig();
    void onUpdateMiniserverClicked();
    void onCancelUpdateClicked();
    void onUpdateMiniserversFinished();
    void onRefreshCancelClicked();
    void onRefreshMiniserversFinished();
    void onConnectConfigFinished();
    void onCancelConnectConfigClicked();
    void onUpdateStatusbarProgress(int progress, QString progresstext);
    void onAddMiniserverPressed();
    void onRemoveMiniserverPressed();
    void onApplicationSettingsClicked();
    void onConfigFilePathChanged();
    void onSaveFileClicked();
    void onOpenFileClicked();
    void onNewFileClicked();
    void onStartAppClicked();
    void onStartDebugAppClicked();
    void onKillLoxoneAppClicked();
    void onVersionClicked();
    void onChangelogClicked();
    void onDownloadProgFolder(CMiniserver ms);
    void onDeselectAll();
    void onCheckNewVersionClicked();



signals:
    void updateStatusbarProgress(int progress, QString progresstext);
private:
    QList<CMiniserver>* miniservers;
    ApplicationSettings* applicationSettings;

    QVBoxLayout* vBox;
    Qt_Menubar* menubar;
    Qt_MiniserverTableView* tableViewMiniserver;
    Qt_Bottom_Action_Buttons* bottom_buttons;
    Qt_Statusbar* statusbar; 
    QWidget* centralWidget; 
    CUpdateWorker* updateWorker;
    CRefreshWorker* refreshWorker;
    CConnectConfigWorker* connectConfigWorker;
    CDownloadProgFolderWorker* downloadProgFolderWorker;
    QString applicationVersion;
    QAction* actionDeselectAll;
    QAction* actionRemoveMiniserverWithDelete;
    CUpdateChecker* checkUpdater;

    //CConfigMSUpdate* configMSUpdate;
    //Ui::Qt_MiniserverUpdaterClass ui;
};


