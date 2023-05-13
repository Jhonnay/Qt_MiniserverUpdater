#include "Qt_MiniserverUpdater.h"
#include "CWebService.h"
#include "MyConstants.h"

Qt_MiniserverUpdater::Qt_MiniserverUpdater(QWidget* parent )
    : QMainWindow(parent)
{}

Qt_MiniserverUpdater::Qt_MiniserverUpdater(QList<CMiniserver>* miniserverList, QWidget *parent )
    : QMainWindow(parent)
{
    miniservers = miniserverList;
    vBox = new QVBoxLayout(this);
    menubar = new Qt_Menubar();
    tableViewMiniserver = new Qt_MiniserverTableView(miniservers,this);
    bottom_buttons = new Qt_Bottom_Action_Buttons(); 
    statusbar = new Qt_Statusbar();
    applicationSettings = NULL;
    

    vBox->addWidget(menubar);
    vBox->addWidget(tableViewMiniserver);
    vBox->addWidget(bottom_buttons);
    vBox->addWidget(statusbar);
    vBox->setSpacing(0);
    vBox->setContentsMargins(0, 0, 0, 0);

    centralWidget = new QWidget();
    centralWidget->setLayout(vBox);

    
    //connect(tableViewMiniserver, &Qt_MiniserverTableView::connectConfigClicked, this, &Qt_MiniserverUpdater::onConnectConfigClicked);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonRefreshClicked, this, &Qt_MiniserverUpdater::onRefreshClicked);
    //connect(tableViewMiniserver, &Qt_MiniserverTableView::localIPTextChanged, this, &Qt_MiniserverUpdater::onLocalIPTextChanged);

    this->setCentralWidget(centralWidget);


    //ui.setupUi(this);
}

Qt_MiniserverUpdater::~Qt_MiniserverUpdater()
{} //ist das bei MainWindow noetig?

void Qt_MiniserverUpdater::setMiniserverList(QList<CMiniserver>* list)
{
    for (int i = 0; i < list->count(); i++) {
        (*list)[i].setMiniserverStatus(MyConstants::Strings::StartUp_Listview_MS_Status);
        (*list)[i].setMiniserverVersion(MyConstants::Strings::StartUp_Listview_MS_Version);
        (*list)[i].setVersionColor("darkblue");
    }
    this->miniservers = list;
    tableViewMiniserver->setModel(new CMiniserverTableModel(list,this));
    tableViewMiniserver->resizeColumnsToContents();
}

void Qt_MiniserverUpdater::setConfigEXEPath(QString path)
{
    statusbar->setConfigExePath(path);
}





void Qt_MiniserverUpdater::onRefreshClicked()
{
    const QModelIndexList selectedIndexes =  tableViewMiniserver->selectionModel()->selectedRows();

    for (const QModelIndex& index : selectedIndexes)
    {
        // Get the row number of the selected index
        int row = index.row();

        // Get the data for the selected row
        CMiniserver& miniserver = tableViewMiniserver->getMiniserverModel()->miniserverlist->operator[](index.row());
        qDebug() << "Selected row: " << miniserver.getSerialNumber().c_str();
        if (!miniserver.getLocalIP().empty()) {
            QString unformatedVersionString = CWebService::sendCommandRest_Version_Local_Gen1(miniserver, "dev/sys/version", "value");
            miniserver.setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
        }
        else {
            QString unformatedVersionString  = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "dev/sys/version", "value");
            miniserver.setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
        }
        emit tableViewMiniserver->getMiniserverModel()->dataChanged(index, index);
    }

    tableViewMiniserver->update();

    qDebug() << "Printing all miniservers after RefreshButton was clicked!";
    for (int i = 0; i < miniservers->count(); i++) {
        qDebug() << "Row: " << i << " - " << miniservers->at(i).toString();
    }

}

void Qt_MiniserverUpdater::onConnectConfig()
{


}

void Qt_MiniserverUpdater::onConnectConfigClicked(CMiniserver* miniserver) {
    qDebug() << miniserver->toString();
}


