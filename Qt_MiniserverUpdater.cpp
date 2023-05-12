#include "Qt_MiniserverUpdater.h"
#include "CWebService.h"

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
    this->miniservers = list;
    tableViewMiniserver->setModel(new CMiniserverTableModel(list,this));
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
    //tableViewMiniserver->sortByColumn(-1, Qt::AscendingOrder);
    //tableViewMiniserver->update();
    //QList<QTreeWidgetItem*>  selected = tableViewMiniserver->selectedItems();
    //QList<QString> selectedSerialNumbers = getSelectedSerialNumbers(selected);

    ////tableViewMiniserver->setMiniservers(miniservers); //set everyone to TBD and outdated info. 
    //tableViewMiniserver->setDisabled(true);
    //bottom_buttons->setDisabledAllExceptCancel(true);

    //for (int i = 0; i < selectedSerialNumbers.count(); ++i){
    //
    //    int realIndex = tableViewMiniserver->getRealIndexfromSerialNumber(selectedSerialNumbers[i]); //here might be problem
    //    if (!(*miniservers)[realIndex].getLocalIP().empty()) {
    //        QString unformatedVersionString = CWebService::sendCommandRest_Version_Local_Gen1((*miniservers)[realIndex], "dev/sys/version", "value");
    //        (*miniservers)[realIndex].setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
    //    }
    //    else {
    //        QString unformatedVersionString  = CWebService::sendCommandRest_Version_Remote_Cloud((*miniservers)[realIndex], "dev/sys/version", "value");
    //        (*miniservers)[realIndex].setMiniserverVersion(CMiniserver::formatMiniserverVersionQString(unformatedVersionString).toStdString());
    //    }
    //   
    //}
 
    //tableViewMiniserver->setMiniserversUpdateContents(miniservers);
    //tableViewMiniserver->setDisabled(false);
    //bottom_buttons->setDisabledAllExceptCancel(false);
    
    
    

}

void Qt_MiniserverUpdater::onLocalIPTextChanged(QTreeWidgetItem* item)
{
    ////TODO: Connect / Load / LOUT
    //int rowIndex = tableViewMiniserver->indexFromItem(item).row();
    //QString serialnumber = item->text(1);
    //int realIndex = tableViewMiniserver->getRealIndexfromSerialNumber(serialnumber);
    //
    //QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->treeWidget()->itemWidget(item, 7));
    //if (lineEdit) {
    //    QString ipAddress = lineEdit->text();
    //    (*miniservers)[realIndex].setLocalIP(ipAddress.toStdString());
    //    std::string message = "Local IP  changed to: " + serialnumber.toStdString() + "Index: " + std::to_string(rowIndex) + " RealIndex: " + std::to_string(realIndex);
    //    qDebug() << message;
    //    CMiniserver::printAllMiniserversToDebug(miniservers);
    //    return; //if everything was OK. 
    //}

    ////TODO: Show Dialog
    ////if comboboxitem = Null OR LanguageList does not contain String
    //std::string message = "Could not change IP of real Index: " + realIndex;
    //qDebug() << message;
}

void Qt_MiniserverUpdater::onConnectConfigClicked(QTreeWidgetItem* item) {    
    ////TODO: Connect / Load / LOUT
    //int index = tableViewMiniserver->indexFromItem(item).row();
    //std::string message = "Connect Config Button clicked! Index: " + std::to_string(index);
    //qDebug() << message;

    //Qt_ComboBoxItem* comboBoxItem = qobject_cast<Qt_ComboBoxItem*>(item->treeWidget()->itemWidget(item, 8));
    //if (comboBoxItem) {
    //    QString comboBoxText = comboBoxItem->currentText();
    //    if (CConfig::LanguageList.contains(comboBoxText)) {
    //        QString LanguageListIndex = QString::number(CConfig::LanguageList.indexOf(comboBoxText));
    //        CConfig::startConfig_Language(statusbar->getConfigExePath(), LanguageListIndex);
    //        return; //if everything was OK. 
    //    }
    //}
    //
    ////TODO: Show Dialog
    ////if comboboxitem = Null OR LanguageList does not contain String
    //CConfig::startConfig_Language(statusbar->getConfigExePath(), NULL);

}


