#include "stdafx.h"
#include <windows.h>
#include <QDesktopServices>
#include "Qt_CleanOldConfigs.h"
#include <QtWidgets/QApplication>
#include <stdio.h>

int main(int argc, char* argv[])
{
    QString programFilesPath = QProcessEnvironment::systemEnvironment().value("ProgramFiles(x86)") + "\\Loxone";

    QApplication a(argc, argv);
    QDir programdata(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1)); // "C:/ProgramData/<APPNAME>"
    programdata.cdUp(); // cd .. "C:/ProgramData"
    programdata.cd("Loxone");

    Qt_CleanOldConfigs dialog = Qt_CleanOldConfigs(QObject::tr("Config Cleaner"), programFilesPath, programdata.path());
    QIcon icon = QIcon("resources/QtConfigCleaner.ico");
    if (!icon.isNull()) {
        dialog.setWindowIcon(icon);
    }
    else {
        qDebug() << "Failed to load the icon.";
    }
    dialog.setWindowIcon(QIcon("resources/QtConfigCleaner.ico"));

   

    return dialog.exec();
}