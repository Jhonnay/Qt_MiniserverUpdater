#include "stdafx.h"
#include "Qt_Miniserver_Listview.h"
#include "Qt_MiniserverUpdater.h"



Qt_Miniserver_Listview::Qt_Miniserver_Listview(QWidget* parent)
    : QTreeWidget(parent)
{
    this->setRootIsDecorated(false);
    this->setAlternatingRowColors(false);
    this->setSortingEnabled(true);
    this->setSelectionMode(MultiSelection);
    this->setContentsMargins(2,2,2,2);
    this->setupColumns();
    

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
    QPushButton* button = new QPushButton("Connect Config", this);
    connect(button, &QPushButton::clicked, this, [this, item]() {
        emit connectConfigClicked(item);
        });
    setItemWidget(item, 6, button);

    // Create a combo box in the "Language" column
    
    
    item->setText(7, QString::fromStdString(miniserver.getLocalIP()));
    Qt_ComboBoxItem* comboBoxLanguage = new Qt_ComboBoxItem(item, 8);
    connect(comboBoxLanguage, &Qt_ComboBoxItem::currentTextChanged, this, [this, item]() {
        emit comboBoxLanguageChanged(item, 8);
        });
    setItemWidget(item, 8, comboBoxLanguage);
    comboBoxLanguage->setCurrentIndex(std::stoi(miniserver.getConfigLanguage()));
    item->setForeground(2, QColor(miniserver.getVersionColor().c_str()));

    addTopLevelItem(item);
}



void Qt_Miniserver_Listview::setMiniservers(QList<CMiniserver>* list)
{
    
    if (list) {
        clear();
        asignMiniserverList(list);
        int index = 0;
        for (auto& miniserver : *list) {
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

void Qt_Miniserver_Listview::comboBoxLanguageChanged(QTreeWidgetItem* item, int column = 8)
{
    int index = this->indexFromItem(item).row();
    QString strSerialNumber = item->text(1);
    int realIndex = getRealIndexfromSerialNumber(strSerialNumber);
    Qt_ComboBoxItem* comboBoxItem = qobject_cast<Qt_ComboBoxItem*>(item->treeWidget()->itemWidget(item, column));


    if (index >= 0 && index < miniserverlist->size()) {
        if (comboBoxItem) {
            QString comboBoxText = comboBoxItem->currentText();
            std::string message = "Language changed:" + comboBoxText.toStdString() + " Index: " + std::to_string(index) + " Real Index: " + std::to_string(realIndex)  + "\n";
            wchar_t wideMessage[512];
            MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, wideMessage, 512);
            OutputDebugString(wideMessage);
            if (CConfig::LanguageList.contains(comboBoxText)) {
                QString LanguageListIndex = QString::number(CConfig::LanguageList.indexOf(comboBoxText));
                (*miniserverlist)[realIndex].setConfigLanguage(LanguageListIndex.toStdString());
                for (int i = 0; i < miniserverlist->count(); ++i)
                {
                    std::string message = " Index: " + std::to_string(i) + " - " + (*miniserverlist)[i].toString() + "\n";
                    wchar_t wideMessage[512];
                    MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, wideMessage, 512);
                    OutputDebugString(wideMessage);
                }

                return;
            }
        }
    }
      
    //TODO: Show Dialog
    std::string message = " ERROR! : Language could not be changed at Index" + std::to_string(index) + "\n";
    wchar_t wideMessage[512];
    MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, wideMessage, 512);
    OutputDebugString(wideMessage);
   
    
}

int Qt_Miniserver_Listview::getRealIndexfromSerialNumber(QString serialnumber)
{
    for (int i = 0; i < miniserverlist->count(); ++i) {
        if ((*miniserverlist)[i].getSerialNumber()._Equal(serialnumber.toStdString()) ){
            return i;
        }
    }


    return -1;
}

void Qt_Miniserver_Listview::RefreshButtonClicked(){
}

Qt_Miniserver_Listview::~Qt_Miniserver_Listview()
{}


