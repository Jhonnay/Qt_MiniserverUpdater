#include "Qt_MiniserverUpdater.h"

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

    this->setCentralWidget(centralWidget);


    //ui.setupUi(this);
}

Qt_MiniserverUpdater::~Qt_MiniserverUpdater()
{} //ist das bei MainWindow noetig?

void Qt_MiniserverUpdater::setMiniserverList(QList<CMiniserver>* list)
{
    treeViewMiniserver->setMiniservers(list);
}

void Qt_MiniserverUpdater::setConfigEXEPath(QString path)
{
    statusbar->setConfigExePath(path);
}

void Qt_MiniserverUpdater::onConnectRefreshClicked()
{
    QList<QTreeWidgetItem*> selected = treeViewMiniserver->selectedItems();

}

void Qt_MiniserverUpdater::onConnectConfigClicked(QTreeWidgetItem* item) {
    
    int index = treeViewMiniserver->indexFromItem(item).row();
    std::string message = "Connect Config Button clicked! Index: " + std::to_string(index);
    wchar_t wideMessage[256];
    MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, wideMessage, 256);
    OutputDebugString(wideMessage);

    Qt_ComboBoxItem* comboBoxItem = qobject_cast<Qt_ComboBoxItem*>(item->treeWidget()->itemWidget(item, 8));
    if (comboBoxItem) {
        QString comboBoxText = comboBoxItem->currentText();
        if (CConfig::LanguageList.contains(comboBoxText)) {
            QString LanguageListIndex = QString::number(CConfig::LanguageList.indexOf(comboBoxText));
            CConfig::startConfig_Language(statusbar->getConfigExePath(), LanguageListIndex);
            return;
        }
    }
    
    //if comboboxitem = Null OR LanguageList does not contain String
    CConfig::startConfig_Language(statusbar->getConfigExePath(), NULL);

}


