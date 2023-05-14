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

class Qt_MiniserverUpdater : public QMainWindow
{
    Q_OBJECT

public:
    Qt_MiniserverUpdater(QWidget* parent = nullptr);
    Qt_MiniserverUpdater(QList<CMiniserver>* miniserverList, QWidget* parent = nullptr);
    ~Qt_MiniserverUpdater();
    
    void setMiniserverList(QList<CMiniserver>* list);
    void setConfigEXEPath(QString path);
    
    

public slots:
    void onConnectConfigClicked(const QModelIndex& index, const CMiniserver& miniserver);
   
    void onRefreshClicked();
    void onConnectConfig();


private:
    QList<CMiniserver>* miniservers;
    ApplicationSettings* applicationSettings;

    QVBoxLayout* vBox;
    Qt_Menubar* menubar;
    Qt_MiniserverTableView* tableViewMiniserver;
    Qt_Bottom_Action_Buttons* bottom_buttons;
    Qt_Statusbar* statusbar; 
    QWidget* centralWidget; 
    //Ui::Qt_MiniserverUpdaterClass ui;
};


