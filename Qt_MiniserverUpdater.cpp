#include "Qt_MiniserverUpdater.h"

Qt_MiniserverUpdater::Qt_MiniserverUpdater(QWidget *parent)
    : QMainWindow(parent)
{

    vBox = new QVBoxLayout(this);
    menubar = new Qt_Menubar();
    treeViewMiniserver = new Qt_Miniserver_Listview();
    bottom_buttons = new Qt_Bottom_Action_Buttons(); 
    statusbar = new Qt_Statusbar();
    applicationSettings = NULL;

    vBox->addWidget(menubar);
    vBox->addWidget(treeViewMiniserver);
    vBox->addWidget(bottom_buttons);
    vBox->addWidget(statusbar);
    vBox->setSpacing(0);
    vBox->setContentsMargins(0, 0, 0, 0);

    centralWidget = new QWidget();
    centralWidget->setLayout(vBox);
    this->setCentralWidget(centralWidget);


    //ui.setupUi(this);
}

Qt_MiniserverUpdater::~Qt_MiniserverUpdater()
{} //ist das bei MainWindow noetig?

void Qt_MiniserverUpdater::setMiniserverList(QList<CMiniserver>* list)
{
    treeViewMiniserver->setMiniservers(miniservers);
}

void Qt_MiniserverUpdater::setConfigEXEPath(QString path)
{
    statusbar->setConfigExePath(path);
}


