#pragma once
#include "stdafx.h"
#include <QtWidgets/QMainWindow>
#include "ui_Qt_MiniserverUpdater.h"
#include "CMiniserver.h"
#include "CMiniserver.h"
#include "Qt_ComboBoxItem.h"
#include "MyConstants.h"


class Qt_Miniserver_Listview : public QTreeWidget
{
    Q_OBJECT

public:
    explicit Qt_Miniserver_Listview(QWidget* parent = nullptr);
    void addMiniserverInitial(const CMiniserver& miniserver,int index);
    void addMiniserverUpdateContent(const CMiniserver& miniserver, int index);
    void setMiniserversInitial(QList<CMiniserver>* list);
    void setMiniserversUpdateContents(QList<CMiniserver>* list);
    void asignMiniserverList(QList<CMiniserver>* miniservers);
    int getRealIndexfromSerialNumber(QString serialnumber);
    ~Qt_Miniserver_Listview();

public slots:
    //void miniserverlistChanged();
   
    void RefreshButtonClicked();

signals:
    void connectConfigClicked(QTreeWidgetItem* item);
    void localIPTextChanged(QTreeWidgetItem* item);

private:

    QList<CMiniserver>* miniserverlist;
    void setupColumns();
    void comboBoxLanguageChanged(QTreeWidgetItem* item, int column);

    void NewFunction();
    


};
