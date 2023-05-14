#pragma once
#include "stdafx.h"
#include "CMiniserver.h"

class CMiniserverTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CMiniserverTableModel(QList<CMiniserver>* miniservers, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    
    QList<CMiniserver>* miniserverlist;
    


signals:
    void connectConfigClicked(const CMiniserver& miniserver);

public slots:
    void onConnectConfigClicked();
    //void sort(int column, Qt::SortOrder order);

    
};

