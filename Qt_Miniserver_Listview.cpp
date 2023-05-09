#include "stdafx.h"
#include "Qt_Miniserver_Listview.h"
#include "Qt_MiniserverUpdater.h"


Qt_Miniserver_Listview::Qt_Miniserver_Listview(QWidget* parent)
    : QTreeWidget(parent)
{
    this->setRootIsDecorated(false);
    this->setAlternatingRowColors(false);
    this->setSortingEnabled(true);
    setupColumns();

}

void Qt_Miniserver_Listview::addMiniserver(const CMiniserver& miniserver,int index) 
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this);
    item->setText(0, QString::fromStdString(MyConstants::Strings::StartUp_Listview_MS_Status));
    item->setText(1, QString::fromStdString(miniserver.getSerialNumber()));
    item->setText(2, QString::fromStdString(MyConstants::Strings::StartUp_Listview_MS_Version));
    item->setText(3, QString::fromStdString(miniserver.getMiniserverProject()));
    item->setText(4, QString::fromStdString(miniserver.getMiniserverConfiguration()));
    item->setText(5, QString::fromStdString(miniserver.getUpdatelevel()));

    // Create a push button in the "Action" column
    QPushButton* button = new QPushButton("Action", this);
    //connect(button, &QPushButton::clicked, parent, &Qt_MiniserverUpdater->onActionClicked);
    setItemWidget(item, 6, button);

    // Create a combo box in the "Language" column
    Qt_ComboBoxItem* comboBoxLanguage = new Qt_ComboBoxItem(this);
    
    item->setText(7, QString::fromStdString(miniserver.getLocalIP()));
    setItemWidget(item, 8, comboBoxLanguage);
    item->setForeground(2, QColor(miniserver.getVersionColor().c_str()));

    addTopLevelItem(item);
}



void Qt_Miniserver_Listview::setMiniservers(QList<CMiniserver>* list)
{
    
    if (list) {
        clear();
        asignMiniserverList(list);
        int index = 0;
        for (const auto& miniserver : *list) {
            // Access the current CMiniserver object via the miniserver variable
            // e.g. miniserver.someMethod();
            addMiniserver(miniserver, index);
            index++;
        }
        update();
    }
        
}


    

void Qt_Miniserver_Listview::asignMiniserverList(QList<CMiniserver>* miniservers)
{
    this->miniserverlist = miniservers;
}


void Qt_Miniserver_Listview::setupColumns() {
    this->setColumnCount(9);
    this->setHeaderLabels({ "Status", "Miniserver", "Version", "Project", "Configuration", "UpdateLevel", "Action", "Local IP", "Language" });
}

void Qt_Miniserver_Listview::RefreshButtonClicked(){
}

Qt_Miniserver_Listview::~Qt_Miniserver_Listview()
{}


