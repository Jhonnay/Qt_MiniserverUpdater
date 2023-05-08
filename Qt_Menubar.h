#pragma once

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

class Qt_Menubar : public QWidget
{
    Q_OBJECT

public:
    explicit Qt_Menubar(QWidget* parent = nullptr);
    ~Qt_Menubar();

signals:

private:
    void createFileMenu();
    void createSettingsMenu();
    void createLoxoneAppMenu();
    void createHelpMenu();

    QMenuBar* menuBar;
    QMenu* fileMenu;
    QMenu* settingsMenu;
    QMenu* loxoneAppMenu;
    QMenu* helpMenu;

    QAction* saveAct;
    QAction* openAct;
    QAction* newAct;
    QAction* exitAct;

    QAction* settingsAct;

    QAction* startAppAct;
    QAction* debugAppAct;
    QAction* killAppAct;

    QAction* versionAct;
    QAction* checkUpdateAct;
    QAction* helpAct;
    QAction* changelogAct;
};
