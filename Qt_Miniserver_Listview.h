#pragma once
#include "stdafx.h"
#include <QtWidgets/QMainWindow>
#include "ui_Qt_MiniserverUpdater.h"
#include "CMiniserver.h"


class Qt_Miniserver_Listview : public QWidget
{
    Q_OBJECT

public:
    explicit Qt_Miniserver_Listview(QWidget* parent = nullptr);
    void setData(std::vector<CMiniserver> data);
    void addCMiniserverToModel(const CMiniserver& miniserver, QStandardItemModel* model);
    void asignMiniserverList(QList<CMiniserver> miniservers);
    ~Qt_Miniserver_Listview();

public slots:
    //void miniserverlistChanged();
    
    void RefreshButtonClicked();

private:
    QVBoxLayout* vBox;
    QTreeView* treeView;
    QList<CMiniserver>* miniserverlist;
};
