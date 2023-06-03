#include "CMiniserverTableModel.h"
#include "CConnectConfigButtonDelegate.h"

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
        case 8: return CConfig::LanguageList.at(stoi(miniserver.getConfigLanguage()));
        }
    }
    else if (role == Qt::ForegroundRole && index.column() == 2)
    {
        return QColor(QString::fromStdString(miniserver.getVersionColor()));
    }
    else if (role == Qt::FontRole && index.column() == 2) {
        QFont font;
        font.setBold(true);
        return font;
    }else if (role == Qt::TextAlignmentRole && (index.column() == 2 || index.column() == 4 || index.column() == 5)) {
        // center the text for column 1 and 3
        return Qt::AlignCenter;
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
        case 1: return "Miniserver";
        case 2: return "Version";
        case 3: return "Project";
        case 4: return "Configuration";
        case 5: return "Update Level";
        case 6: return "Action";
        case 7: return "Local IP";
        case 8: return "Language";
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
        if (index.column() == 6 ||index.column() == 7 || index.column() == 8)
            return defaultFlags | Qt::ItemIsEditable;
    }

    return defaultFlags;
}



bool CMiniserverTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        // Check if the passed value is a CMiniserver object
        QVariant variant = value;
        if (variant.canConvert<CMiniserver>())
        {
            CMiniserver miniserver = variant.value<CMiniserver>();
            miniserverlist->replace(index.row(), miniserver);
            emit dataChanged(this->index(index.row(), 0), this->index(index.row(), 8));
            
            printDebugDataChanged(index, miniserver);
            return true;
        }
        else {
            CMiniserver& miniserver = miniserverlist->operator[](index.row());

            switch (index.column())
            {
            case 7: miniserver.setLocalIP(value.toString().toStdString()); break;
            case 8: miniserver.setConfigLanguage(value.toString().toStdString()); break;
            default: return false;
            }
            printDebugDataChanged(index, miniserver);

            //emit dataChanged(index, index);
            emit dataChanged(this->index(index.row(), 0), this->index(index.row(), 8));

            return true;
        }
    }

    return false;
}

bool CMiniserverTableModel::insertRow(const CMiniserver& miniserver) {
    int row = miniserverlist->size();
    beginResetModel();
    beginInsertRows(QModelIndex(), row, row);
    
    
    miniserverlist->append(miniserver);
    endInsertRows();
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
    layoutChanged();
    endResetModel();
    setData(this->index(row,0), QVariant::fromValue(miniserver), Qt::EditRole);
    return true;
}

void CMiniserverTableModel::printDebugDataChanged(const QModelIndex& index, CMiniserver& miniserver)
{
    qDebug() << "Data Changed @ Row:  " << index.row() << " Column: " << index.column() << " - " << miniserver.toString();

    for (int i = 0; i < miniserverlist->count(); i++) {
        qDebug() << "Row: " << i << " - " << miniserverlist->at(i).toString();
    }
}

void CMiniserverTableModel::onConnectConfigClicked()
{
    // Get the row and column of the clicked button
    QModelIndex index = qobject_cast<QPushButton*>(sender())->property("index").value<QModelIndex>();
    int row = index.row();

    // Get the corresponding miniserver
    const CMiniserver& miniserver = miniserverlist->at(row);

    // Emit the signal
    emit connectConfigClicked(miniserver);
}




//void CMiniserverTableModel::sort(int column, Qt::SortOrder order)
//{
//    // Sort the list of CMiniserver objects based on the selected column
//    std::sort(miniserverlist->begin(), miniserverlist->end(), [&](const CMiniserver& msA, const CMiniserver& msB) {
//        switch (column)
//        {
//        case 0: return msA.getMiniserverStatus() < msB.getMiniserverStatus();
//        case 1: return msA.getSerialNumber() < msB.getSerialNumber();
//        case 2: return msA.getMiniserverVersion() < msB.getMiniserverVersion();
//        case 3: return msA.getMiniserverProject() < msB.getMiniserverProject();
//        case 4: return msA.getMiniserverConfiguration() < msB.getMiniserverConfiguration();
//        case 5: return msA.getUpdatelevel() < msB.getUpdatelevel();
//        case 6: return false; // do not sort on Connect Config column
//        case 7: return msA.getLocalIP() < msB.getLocalIP();
//        case 8: return msA.getConfigLanguage() < msB.getConfigLanguage();
//        default: return false;
//        }
//        });
//
//    if (order == Qt::DescendingOrder)
//    {
//        std::reverse(miniserverlist->begin(), miniserverlist->end());
//    }
//
//    // Emit the layoutChanged signal to notify the view that the data has been sorted
//    emit layoutChanged();
//}