#pragma once


#include "ui_Qt_Statusbar.h"
#include "stdafx.h"


#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>
#include <iostream>

class Qt_Statusbar : public QWidget
{
    Q_OBJECT

public:
    Qt_Statusbar(QWidget* parent = nullptr);
    QString getSelectedFilePath() const;

public slots:
    QString getConfigExePath();
    void setConfigExePath(QString filepath);

private slots:
    void selectFile();
    void updateProgress(int progress, std::string progresstext);
    bool checkConfigEXE(QString configFilePath);
    void startConfig(QString configPath);
    void startConfig_Language(QString configPath, QString Language);
    void killAllConfigs();




    //QString getVersionString(QString fName);

signals:
    void doubleClicked();
protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QStatusBar* statusbar;
    QLabel* fileLabel;
    QLabel* progressLabel;
    QProgressBar* progressBar;
    QString exefilePath;
    QHBoxLayout* hbox;
    QAction* startConfigAction;
    QMenu* startConfigMenu;
    QAction* killConfigAction;
    QMenu* contextMenu;

};

