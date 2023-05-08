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
    ~Qt_Miniserver_Listview();

public slots:
    //void miniserverlistChanged();

private:
    QVBoxLayout* vBox;
    QTreeView* treeView;
};
