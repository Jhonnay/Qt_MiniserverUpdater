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

void Qt_Menubar::onSaveClicked() {
    emit saveClicked();
}

void Qt_Menubar::onOpenClicked() {
    emit openClicked();
}

void Qt_Menubar::onNewClicked()
{


    emit newClicked();
}

void Qt_Menubar::onStartAppClicked()
{
    emit startAppClicked();
}

void Qt_Menubar::onStartDebugAppClicked()
{
    emit debugAppClicked();
}

void Qt_Menubar::onKillLoxoneAppClicked()
{
    emit killAppClicked();
}

void Qt_Menubar::onVersionClicked()
{
    emit versionClicked();
}

void Qt_Menubar::onChangelogClicked()
{
    emit changelogClicked();
}




void Qt_Menubar::createFileMenu()
{
    fileMenu = menuBar->addMenu(tr("&File"));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &Qt_Menubar::onSaveClicked);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &Qt_Menubar::onOpenClicked);

    newAct = new QAction(tr("&New"), this);
    newAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    fileMenu->addAction(newAct);
    connect(newAct, &QAction::triggered, this, &Qt_Menubar::onNewClicked);

    fileMenu->addSeparator();

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
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

    startAppAct = new QAction(tr("&✈ Start App"), this);
    loxoneAppMenu->addAction(startAppAct);
    connect(startAppAct, &QAction::triggered, this, &Qt_Menubar::onStartAppClicked);

    debugAppAct = new QAction(tr("&🖥 Debug"), this);
    loxoneAppMenu->addAction(debugAppAct);
    connect(debugAppAct, &QAction::triggered, this, &Qt_Menubar::onStartDebugAppClicked);

    killAppAct = new QAction(tr("&💣 Kill App"), this);
    loxoneAppMenu->addAction(killAppAct);
    connect(killAppAct, &QAction::triggered, this, &Qt_Menubar::onKillLoxoneAppClicked);
}

void Qt_Menubar::createHelpMenu()
{
    helpMenu = menuBar->addMenu(tr("&Help"));

    versionAct = new QAction(tr("&Version"), this);
    helpMenu->addAction(versionAct);
    connect(versionAct, &QAction::triggered, this, &Qt_Menubar::onVersionClicked);

    checkUpdateAct = new QAction(tr("&Check Update"), this);
    helpMenu->addAction(checkUpdateAct);

    helpAct = new QAction(tr("&Help"), this);
    helpMenu->addAction(helpAct);

    changelogAct = new QAction(tr("&Changelog"), this);
    helpMenu->addAction(changelogAct);
    connect(changelogAct, &QAction::triggered, this, &Qt_Menubar::onChangelogClicked);
}
