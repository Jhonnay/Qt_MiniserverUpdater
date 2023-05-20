#include "stdafx.h"
#include "Qt_Menubar.h"

Qt_Menubar::Qt_Menubar(QWidget* parent)
    : QWidget(parent)
{
    menuBar = new QMenuBar(this);

    createFileMenu();
    createSettingsMenu();
    createLoxoneAppMenu();
    createHelpMenu();

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(menuBar);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

}

Qt_Menubar::~Qt_Menubar()
{
    delete menuBar;
}

void Qt_Menubar::onapplicationSettingsClicked()
{
    emit applicationSettingsClicked();
}

void Qt_Menubar::createFileMenu()
{
    fileMenu = menuBar->addMenu(tr("&File"));

    saveAct = new QAction(tr("&Save"), this);
    fileMenu->addAction(saveAct);

    openAct = new QAction(tr("&Open"), this);
    fileMenu->addAction(openAct);

    newAct = new QAction(tr("&New"), this);
    fileMenu->addAction(newAct);

    fileMenu->addSeparator();

    exitAct = new QAction(tr("&Exit"), this);
    fileMenu->addAction(exitAct);

    connect(exitAct, &QAction::triggered, qApp, QApplication::quit);
}

void Qt_Menubar::createSettingsMenu()
{
    settingsMenu = menuBar->addMenu(tr("&Settings"));

    settingsAct = new QAction(tr("&Startup Settings"), this);
    settingsMenu->addAction(settingsAct);
    connect(settingsMenu, &QMenu::triggered, this, &Qt_Menubar::onapplicationSettingsClicked);

}

void Qt_Menubar::createLoxoneAppMenu()
{
    loxoneAppMenu = menuBar->addMenu(tr("&Loxone App"));

    startAppAct = new QAction(tr("&Start App"), this);
    loxoneAppMenu->addAction(startAppAct);

    debugAppAct = new QAction(tr("&Debug"), this);
    loxoneAppMenu->addAction(debugAppAct);

    killAppAct = new QAction(tr("&Kill App"), this);
    loxoneAppMenu->addAction(killAppAct);
}

void Qt_Menubar::createHelpMenu()
{
    helpMenu = menuBar->addMenu(tr("&Help"));

    versionAct = new QAction(tr("&Version"), this);
    helpMenu->addAction(versionAct);

    checkUpdateAct = new QAction(tr("&Check Update"), this);
    helpMenu->addAction(checkUpdateAct);

    helpAct = new QAction(tr("&Help"), this);
    helpMenu->addAction(helpAct);

    changelogAct = new QAction(tr("&Changelog"), this);
    helpMenu->addAction(changelogAct);
}
