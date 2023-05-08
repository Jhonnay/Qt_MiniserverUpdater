#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt_MiniserverUpdater.h"

class Qt_MiniserverUpdater : public QMainWindow
{
    Q_OBJECT

public:
    Qt_MiniserverUpdater(QWidget *parent = nullptr);
    ~Qt_MiniserverUpdater();

private:
    Ui::Qt_MiniserverUpdaterClass ui;
};
