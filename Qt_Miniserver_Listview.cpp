#include "stdafx.h"
#include "Qt_Miniserver_Listview.h"
#include "CMiniserver.h"

Qt_Miniserver_Listview::Qt_Miniserver_Listview(QWidget* parent)
    : QWidget(parent)
{

    vBox = new QVBoxLayout(this);
    treeView = new QTreeView();

    treeView->setRootIsDecorated(false);
    treeView->setAlternatingRowColors(false);
    treeView->setSortingEnabled(true);


    // Add the tree view to the main window's layout
    vBox->addWidget(treeView);

}


void Qt_Miniserver_Listview::setData(std::vector<CMiniserver> data)
{
    QStandardItemModel* model = new QStandardItemModel(0, 9, this);
    model->setHeaderData(0, Qt::Horizontal, "Status");
    model->setHeaderData(1, Qt::Horizontal, "Miniserver");
    model->setHeaderData(2, Qt::Horizontal, "Version");
    model->setHeaderData(3, Qt::Horizontal, "Project");
    model->setHeaderData(4, Qt::Horizontal, "Configuration");
    model->setHeaderData(5, Qt::Horizontal, "UpdateLevel");
    model->setHeaderData(6, Qt::Horizontal, "Action");
    model->setHeaderData(7, Qt::Horizontal, "Local IP");
    model->setHeaderData(8, Qt::Horizontal, "Language");


    for (const auto& miniserver : data) {
        addCMiniserverToModel(miniserver, model);
    }

    treeView->setModel(model);
    treeView->setSelectionMode(QAbstractItemView::MultiSelection);

    for (int i = 0; i < model->columnCount(); i++) {
        treeView->resizeColumnToContents(i);
    }


    //QModelIndexList selection = treeView->selectedIndexes();
    //foreach(QModelIndex index, selection) {
    //    // Access the selected item using the index
    //    // ...
    //}
}


void Qt_Miniserver_Listview::addCMiniserverToModel(const CMiniserver& miniserver, QStandardItemModel* model)
{
    // Create a new row of items for the CMiniserver object
    QList<QStandardItem*> rowItems;

    // Create a QStandardItem for each property in the CMiniserver object
    QStandardItem* status = new QStandardItem(QString::fromStdString(miniserver.getMiniserverStatus()));
    QStandardItem* serialnumber = new QStandardItem(QString::fromStdString(miniserver.getSerialNumber()));
    QStandardItem* version = new QStandardItem(QString::fromStdString(miniserver.getMiniserverVersion()));
    QStandardItem* project = new QStandardItem(QString::fromStdString(miniserver.getMiniserverProject()));
    QStandardItem* configuration = new QStandardItem(QString::fromStdString(miniserver.getMiniserverConfiguration()));
    QStandardItem* updateLevel = new QStandardItem(QString::fromStdString(miniserver.getUpdatelevel()));
    QStandardItem* Action = new QStandardItem(QString::fromStdString("Todo embedded Button"));
    QStandardItem* localIP = new QStandardItem(QString::fromStdString(miniserver.getLocalIP()));
    QStandardItem* language = new QStandardItem(QString::fromStdString(miniserver.getConfigLanguage()));

    status->setEditable(false);
    serialnumber->setEditable(false);
    version->setEditable(false);
    project->setEditable(false);
    configuration->setEditable(false);
    updateLevel->setEditable(false);
    Action->setEditable(false);


    // Add the QStandardItem objects to the row
    rowItems.append(status);
    rowItems.append(serialnumber);
    rowItems.append(version);
    rowItems.append(project);
    rowItems.append(configuration);
    rowItems.append(updateLevel);
    rowItems.append(Action);
    rowItems.append(localIP);
    rowItems.append(language);

    // Add the row to the model
    model->appendRow(rowItems);



}

Qt_Miniserver_Listview::~Qt_Miniserver_Listview()
{}
