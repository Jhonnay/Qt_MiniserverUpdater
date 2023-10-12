#pragma once

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "Qt_MiniserverTableView.h"



class Qt_Menubar : public QWidget
{
    Q_OBJECT

public:
    explicit Qt_Menubar(QWidget* parent = nullptr);
    ~Qt_Menubar();

    void onapplicationSettingsClicked();
    void onSaveClicked();
    void onOpenClicked();
    void onNewClicked(); 
    void onStartAppClicked();
    void onStartDebugAppClicked();
    void onKillLoxoneAppClicked();
    void onVersionClicked();
    void onChangelogClicked();
    void onCheckNewVersionClicked();
    void setMiniserverTableView(Qt_MiniserverTableView* tv);
    void onHelp();
    void onCleanConfigsClicked();


public slots:
    void updateFileMenuState(bool state);
signals:
    void saveClicked();
    void openClicked();
    void newClicked();
    void exitClicked();
    void applicationSettingsClicked();
    void startAppClicked();
    void debugAppClicked();
    void killAppClicked();
    void versionClicked();
    void checkUpdateClicked();
    void helpClicked();
    void changelogClicked();
    void checkVersionClicked();
    void help();
    void cleanConfigClicked();


private:
    void createFileMenu();
    void createSettingsMenu();
    void createLoxoneAppMenu();
    void createHelpMenu();
    void createToolsMenu();

    QMenuBar* menuBar;
    QMenu* fileMenu;
    QMenu* settingsMenu;
    QMenu* toolsMenu;
    QMenu* loxoneAppMenu;
    QMenu* helpMenu;

    QAction* saveAct;
    QAction* openAct;
    QAction* newAct;
    QAction* exitAct;
    QAction* cleanConfigsAct; 

    QAction* settingsAct;

    QAction* startAppAct;
    QAction* debugAppAct;
    QAction* killAppAct;

    QAction* versionAct;
    QAction* checkUpdateAct;
    QAction* helpAct;
    QAction* changelogAct;

    Qt_MiniserverTableView* tableview;
};
