#include "stdafx.h"
#include "Qt_Statusbar.h"
#include <QFile>
#include <iostream>

#include "Windows.h"
#include <QFileInfo>
#include <QCoreApplication>

#include <QFileInfo>
#include <QByteArray>
#include "CConfig.h"
#include <qthread.h>
#include <QtConcurrent/QtConcurrent>
#include "Qt_MiniserverUpdater.h"




Qt_Statusbar::Qt_Statusbar(QWidget* parent)
    : QWidget(parent)
{
    // Create status bar
    statusbar = new QStatusBar(this);
    hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0, 0, 0, 0);
    setLayout(hbox);
    layout()->addWidget(statusbar);

    // Create file label with double click event
    fileLabel = new QLabel(QString::fromStdString("Current Config : not selected - double click to select"), statusbar);
    fileLabel->setToolTip(tr("Double-click to select file"));
    fileLabel->setAlignment(Qt::AlignLeft);
    fileLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    statusbar->addWidget(fileLabel, 1);
    //connect(fileLabel, &QLabel::doubleClicked, this, &Qt_Statusbar::selectFile);

    // Create progress bar and label
    progressLabel = new QLabel(statusbar);
    progressLabel->setAlignment(Qt::AlignRight);
    progressLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    progressLabel->setMaximumSize(200, 20);
    statusbar->addWidget(progressLabel);

    progressBar = new QProgressBar(statusbar);
    progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    progressBar->setFixedSize(200, 20);
    progressBar->setContentsMargins(0, 0, 0, 0);
    statusbar->addWidget(progressBar);

    statusbar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    statusbar->setContentsMargins(0, 0, 0, 0);
    layout()->setContentsMargins(8, 0, 0, 0);
    //layout()->widget()->setContentsMargins(0, 0, 0, 0);

    // Define the context menu actions
    startConfigAction = new QAction("Start Config", this);
    startConfigMenu = new QMenu("Start Config with Language", this);
    killConfigAction = new QAction("Kill Config Instances", this);

    // Generate the submenu from CConfig::LanguageList
    QList<QString> languageList = CConfig::LanguageList;
    for (int i = 0; i < languageList.size(); ++i) {
        QAction* languageAction = new QAction(languageList.at(i), this);
        startConfigMenu->addAction(languageAction);
    }

    // Add the actions to the context menu
    contextMenu = new QMenu(this);
    contextMenu->addAction(startConfigAction);
    contextMenu->addMenu(startConfigMenu);
    contextMenu->addAction(killConfigAction);

    // Set the context menu for the label
    fileLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(fileLabel, &QLabel::customContextMenuRequested, this, [this](const QPoint& pos) {
        QMenu menu;
        QAction* startConfigAction = menu.addAction("✈ Start Config");
        QMenu* languageMenu = menu.addMenu("📖 Start Config Language");

        // Add language options to the submenu
        for (int i = 0; i < CConfig::LanguageList.size(); i++) {
            QAction* languageAction = languageMenu->addAction(CConfig::LanguageList.at(i));
            connect(languageAction, &QAction::triggered, this, [this, i]() {
                startConfig_Language(fileLabel->text(), QString::number(i));
                });
        }

        QAction* killConfigAction = menu.addAction("💣 Kill Config Instances");

        connect(startConfigAction, &QAction::triggered, this, [this]() {
            startConfig(fileLabel->text());
            });

        connect(killConfigAction, &QAction::triggered, this, [this]() {
            killAllConfigs();
            });

        menu.exec(fileLabel->mapToGlobal(pos));
        });

    //set Background Color
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, "lightgrey");
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    // Set initial state
    //updateProgress(100, "Downloading file");



}

QString Qt_Statusbar::getSelectedFilePath()
{
    return exefilePath;
}

void Qt_Statusbar::selectFile()
{
    QString initialDir = "C:\\Program Files (x86)\\Loxone";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Loxone Config"), initialDir, tr("Executable Files (LoxoneConfig.exe)"));
    QCoreApplication::processEvents();
    if (!fileName.isEmpty()) {
        checkConfigEXE(fileName);

        exefilePath = fileName;
        fileLabel->setText(exefilePath);
        emit exefilepathChanged();
    }

}


void Qt_Statusbar::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {

        selectFile();
    }
}

void Qt_Statusbar::updateProgress(int progress, QString progresstext)
{
    progressBar->setValue(progress);
    progressLabel->setText(progresstext);
    QSize size = progressLabel->sizeHint();
    progressLabel->setFixedSize(size);

}

bool Qt_Statusbar::checkConfigEXE(QString configFilePath)
{
    //TODO: implement in c++
    return true;
}

void Qt_Statusbar::startConfig(QString configPath)
{
    //default is ENU
    int index = CConfig::LanguageList.indexOf("ENU");
    QString english_language = QString::number(index);
    //CConfig::startConfig_Language(configPath, QString::number(CConfig::LanguageList.indexOf("ENU")));
    if (configPath != "Current Config : not selected - double click to select") {
        CConfig::startConfig_Language(configPath, english_language);
    }
    else
    {
        QMessageBox::warning(nullptr, "Error", "No Config Exe selected!");
    }
}

void Qt_Statusbar::startConfig_Language(QString configPath, QString Language)
{
    if (configPath != "Current Config : not selected - double click to select") {
        CConfig::startConfig_Language(configPath, Language);
    }
    else
    {
        QMessageBox::warning(nullptr, "Error", "No Config Exe selected!");
    }

}

void Qt_Statusbar::killAllConfigs() {
    CConfig::killAllConfigs();

}

QString Qt_Statusbar::getConfigExePath()
{
    return  QString(fileLabel->text());
}

void Qt_Statusbar::setConfigExePath(QString filepath)
{
    fileLabel->setText(filepath);
}




//QString Qt_Statusbar::getVersionString(QString fName)
//{
//    // first of all, GetFileVersionInfoSize
//    DWORD dwHandle;
//    DWORD dwLen = GetFileVersionInfoSize(fName.toStdWString().c_str(), &dwHandle);
//
//    // GetFileVersionInfo
//    LPVOID lpData = new BYTE[dwLen];
//    if (!GetFileVersionInfo(fName.toStdWString().c_str(), dwHandle, dwLen, lpData))
//    {
//        qDebug() << "error in GetFileVersionInfo";
//        delete[] lpData;
//        return "";
//    }
//
//    // VerQueryValue
//    VS_FIXEDFILEINFO* lpBuffer = NULL;
//    UINT uLen;
//
//    if (!VerQueryValue(lpData,
//        QString("\\").toStdWString().c_str(),
//        (LPVOID*)&lpBuffer,
//        &uLen))
//    {
//
//        qDebug() << "error in VerQueryValue";
//        delete[] lpData;
//        return "";
//    }
//    else
//    {
//        return QString::number((lpBuffer->dwFileVersionMS >> 16) & 0xffff) + "." +
//            QString::number((lpBuffer->dwFileVersionMS) & 0xffff) + "." +
//            QString::number((lpBuffer->dwFileVersionLS >> 16) & 0xffff) + "." +
//            QString::number((lpBuffer->dwFileVersionLS) & 0xffff);
//    }
//}