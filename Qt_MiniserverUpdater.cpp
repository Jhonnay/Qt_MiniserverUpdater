#include "Qt_MiniserverUpdater.h"
#include "CWebService.h"

Qt_MiniserverUpdater::Qt_MiniserverUpdater(QWidget *parent)
    : QMainWindow(parent)
{

    vBox = new QVBoxLayout(this);
    menubar = new Qt_Menubar();
    treeViewMiniserver = new Qt_Miniserver_Listview(this);
    bottom_buttons = new Qt_Bottom_Action_Buttons(); 
    statusbar = new Qt_Statusbar();
    applicationSettings = NULL;
    miniservers = NULL;

    vBox->addWidget(menubar);
    vBox->addWidget(treeViewMiniserver);
    vBox->addWidget(bottom_buttons);
    vBox->addWidget(statusbar);
    vBox->setSpacing(0);
    vBox->setContentsMargins(0, 0, 0, 0);

    centralWidget = new QWidget();
    centralWidget->setLayout(vBox);

    connect(treeViewMiniserver, &Qt_Miniserver_Listview::connectConfigClicked, this, &Qt_MiniserverUpdater::onConnectConfigClicked);
    connect(bottom_buttons, &Qt_Bottom_Action_Buttons::buttonRefreshClicked, this, &Qt_MiniserverUpdater::onRefreshClicked);
    connect(treeViewMiniserver, &Qt_Miniserver_Listview::localIPTextChanged, this, &Qt_MiniserverUpdater::onLocalIPTextChanged);

    this->setCentralWidget(centralWidget);


    //ui.setupUi(this);
}

Qt_MiniserverUpdater::~Qt_MiniserverUpdater()
{} //ist das bei MainWindow noetig?

void Qt_MiniserverUpdater::setMiniserverList(QList<CMiniserver>* list)
{
    this->miniservers = list;
    treeViewMiniserver->setMiniserversInitial(list);
}

void Qt_MiniserverUpdater::setConfigEXEPath(QString path)
{
    statusbar->setConfigExePath(path);
}

QList<QString> Qt_MiniserverUpdater::getSelectedSerialNumbers(QList<QTreeWidgetItem*> selectedItems)
{
    QList<QString> listOfSerialNumbers;
    for (int i = 0; i < selectedItems.count(); ++i) {
        listOfSerialNumbers.append(selectedItems[i]->text(1));
    }
    return listOfSerialNumbers;
}



void Qt_MiniserverUpdater::onRefreshClicked()
{
    treeViewMiniserver->sortByColumn(-1, Qt::AscendingOrder);
    treeViewMiniserver->update();
    QList<QTreeWidgetItem*>  selected = treeViewMiniserver->selectedItems();
    QList<QString> selectedSerialNumbers = getSelectedSerialNumbers(selected);

    //treeViewMiniserver->setMiniservers(miniservers); //set everyone to TBD and outdated info. 
    treeViewMiniserver->setDisabled(true);
    bottom_buttons->setDisabledAllExceptCancel(true);

    for (int i = 0; i < selectedSerialNumbers.count(); ++i){
    
        int realIndex = treeViewMiniserver->getRealIndexfromSerialNumber(selectedSerialNumbers[i]); //here might be problem
        if (!(*miniservers)[realIndex].getLocalIP().empty()) {
            QString unformatedVersionString = CWebService::sendCommandRest_Version_Local_Gen1((*miniservers)[realIndex], "dev/sys/version", "value");
            (*miniservers)[realIndex].setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
        }
        else {
            QString unformatedVersionString  = CWebService::sendCommandRest_Version_Remote_Cloud((*miniservers)[realIndex], "dev/sys/version", "value");
            (*miniservers)[realIndex].setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
        }
       
    }
 
    treeViewMiniserver->setMiniserversUpdateContents(miniservers);
    treeViewMiniserver->setDisabled(false);
    bottom_buttons->setDisabledAllExceptCancel(false);
    
    
    

}

void Qt_MiniserverUpdater::onLocalIPTextChanged(QTreeWidgetItem* item)
{
    //TODO: Connect / Load / LOUT
    int rowIndex = treeViewMiniserver->indexFromItem(item).row();
    QString serialnumber = item->text(1);
    int realIndex = treeViewMiniserver->getRealIndexfromSerialNumber(serialnumber);
    
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->treeWidget()->itemWidget(item, 7));
    if (lineEdit) {
        QString ipAddress = lineEdit->text();
        (*miniservers)[realIndex].setLocalIP(ipAddress.toStdString());
        std::string message = "Local IP  changed to: " + serialnumber.toStdString() + "Index: " + std::to_string(rowIndex) + " RealIndex: " + std::to_string(realIndex);
        qDebug() << message;
        CMiniserver::printAllMiniserversToDebug(miniservers);
        return; //if everything was OK. 
    }

    //TODO: Show Dialog
    //if comboboxitem = Null OR LanguageList does not contain String
    std::string message = "Could not change IP of real Index: " + realIndex;
    qDebug() << message;
}

void Qt_MiniserverUpdater::onConnectConfigClicked(QTreeWidgetItem* item) {    
    //TODO: Connect / Load / LOUT
    int index = treeViewMiniserver->indexFromItem(item).row();
    std::string message = "Connect Config Button clicked! Index: " + std::to_string(index);
    qDebug() << message;

    Qt_ComboBoxItem* comboBoxItem = qobject_cast<Qt_ComboBoxItem*>(item->treeWidget()->itemWidget(item, 8));
    if (comboBoxItem) {
        QString comboBoxText = comboBoxItem->currentText();
        if (CConfig::LanguageList.contains(comboBoxText)) {
            QString LanguageListIndex = QString::number(CConfig::LanguageList.indexOf(comboBoxText));
            CConfig::startConfig_Language(statusbar->getConfigExePath(), LanguageListIndex);
            return; //if everything was OK. 
        }
    }
    
    //TODO: Show Dialog
    //if comboboxitem = Null OR LanguageList does not contain String
    CConfig::startConfig_Language(statusbar->getConfigExePath(), NULL);

}



/* Get Real Index other way

void Qt_Miniserver_Listview::comboBoxLanguageChanged(QTreeWidgetItem* item, int column = 8)
{
    // Get the original model of the tree view
    QAbstractItemModel* model = ui->treeView->model();

    // Create a proxy model to provide a sorted view of the original model
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    // Set the sort column and order on the proxy model
    proxyModel->setSortColumn(column, Qt::AscendingOrder);

    // Get the index of the item in the sorted view
    QModelIndex proxyIndex = proxyModel->mapFromSource(indexFromItem(item));

    // Get the row number of the item in the sorted view
    int row = proxyIndex.row();

    // Use the row number to get the index of the item in the original model
    QModelIndex index = proxyModel->mapToSource(proxyIndex);

    // Use the index to access the data behind the item
    QVariant data = model->data(index);

    // Do something with the data...
}
*/