#include "CMiniserverTableModel.h"

CMiniserverTableModel::CMiniserverTableModel(QList<CMiniserver>* miniservers, QObject* parent)
    : QAbstractTableModel(parent), miniserverlist(miniservers)
{
}

int CMiniserverTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
        return miniserverlist->size();
}

int CMiniserverTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
        return 9;
}

QVariant CMiniserverTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= miniserverlist->size())
        return QVariant();

    const CMiniserver& miniserver = miniserverlist->at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0: return QString::fromStdString(miniserver.getMiniserverStatus());
        case 1: return QString::fromStdString(miniserver.getSerialNumber());
        case 2: return QString::fromStdString(miniserver.getMiniserverVersion());
        case 3: return QString::fromStdString(miniserver.getMiniserverProject());
        case 4: return QString::fromStdString(miniserver.getMiniserverConfiguration());
        case 5: return QString::fromStdString(miniserver.getUpdatelevel());
        case 7: return QString::fromStdString(miniserver.getLocalIP());
        case 8: return QString::fromStdString(miniserver.getConfigLanguage());
        }
    }
    else if (role == Qt::ForegroundRole && index.column() == 2)
    {
        return QColor(QString::fromStdString(miniserver.getVersionColor()));
    }

    return QVariant();
}

QVariant CMiniserverTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0: return "Status";
        case 1: return "Serial Number";
        case 2: return "Version";
        case 3: return "Project";
        case 4: return "Configuration";
        case 5: return "Update Level";
        case 6: return "Action";
        case 7: return "Local IP";
        case 8: return "Config Language";
        default: return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags CMiniserverTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.isValid())
    {
        if (index.column() == 7 || index.column() == 8)
            return defaultFlags | Qt::ItemIsEditable;
    }

    return defaultFlags;
}

bool CMiniserverTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        CMiniserver& miniserver = miniserverlist->operator[](index.row());

        switch (index.column())
        {
        case 7: miniserver.getLocalIP() = value.toString().toStdString(); break;
        case 8: miniserver.getConfigLanguage() = value.toString().toStdString(); break;
        default: return false;
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}