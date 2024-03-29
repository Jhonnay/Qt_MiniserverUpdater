#include "Qt_MiniserverTableView.h"
#include "CSerialNumberHyperlinkDelegate.h"
#include "CWebService.h"
#include "MyConstants.h"
#include "Qt_CreateEditMiniserver.h"


Qt_MiniserverTableView::Qt_MiniserverTableView(QWidget *parent)
	: QTableView(parent)
{
    //Qt_MiniserverTableView(NULL, this); //does not work
}



Qt_MiniserverTableView::Qt_MiniserverTableView(QList<CMiniserver>* miniservers, QWidget* parent)
    : QTableView(parent), m_model(new CMiniserverTableModel(miniservers, this))
{
    setModel(m_model);
    setSelectionMode(QAbstractItemView::MultiSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setMinimumWidth(35);
    verticalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    setSortingEnabled(true);
    setMouseTracking(true);
    //setUpdatesEnabled(true);
    setEditTriggers(QAbstractItemView::AllEditTriggers); //enable single click for edit mode
    //setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::EditKeyPressed);
    horizontalHeader()->setSectionsMovable(true);     //set Header moveable
    QFont font = horizontalHeader()->font();
    font.setBold(true);
    horizontalHeader()->setFont(font);

    connect(horizontalHeader(), &QHeaderView::sortIndicatorChanged, m_model, &CMiniserverTableModel::sort);
    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &Qt_MiniserverTableView::handleSelectionChanged);
   
    //https://stackoverflow.com/questions/18831242/qt-start-editing-of-cell-after-one-click
    //Test
    //QPalette palette;
    //palette.setColor(QPalette::Highlight, QColor(200, 225, 255)); // set light blue color for selection
    //setPalette(palette);
    //QString styleSheet = "QTableView::item:selected { selection-background-color: rgba(255, 255, 255, 10); }";
    //setStyleSheet(styleSheet);
    
    CSerialNumberHyperlinkDelegate* hyperlinkDelegate = new CSerialNumberHyperlinkDelegate(this);
    setItemDelegateForColumn(1, hyperlinkDelegate);

    // Set up the "Connect Config" push button delegate
    CConnectConfigButtonDelegate* buttonDelegate = new CConnectConfigButtonDelegate(this);
    setItemDelegateForColumn(6, buttonDelegate);
    connect(buttonDelegate, &CConnectConfigButtonDelegate::clicked, this, &Qt_MiniserverTableView::handleConnectConfigClicked);
    
    // Set up the config language ComboBox delegate
    CComboBoxLanguageDelegate* comboBoxDelegate = new CComboBoxLanguageDelegate(this);
    setItemDelegateForColumn(8, comboBoxDelegate);
    resizeColumnsToContents();

    //CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(this->model());
    //if (!model) {
    //    return;
    //}    

    this->setStyleSheet(R"""(
        QTableView {
            selection-background-color: transparent; /* Clear the default selection background */
            outline: none;
        }
        QTableView::item {
            padding: 3px 3px 3px 3px;
        }

        QTableView:enabled {
            /* Add your styles for the enabled state here */
            background-color: white;
            color: black;
        }

        /* Styles for disabled state */
        QTableView:disabled {
            /* Add your styles for the disabled state here */
            background-color: pale gray;
            color: gray;
        }

        QTableView::item:selected {
            background-color: rgba(204, 232, 255, 1); /* Light blue with 100% opacity */
            color: inherit;
            
        }
        QTableView::item:focus {
            border: 3px solid rgba(32, 148, 250, 1);
            padding: 0px 0px 0px 0px ;
        }
      
        

	)""");

}


Qt_MiniserverTableView::~Qt_MiniserverTableView()
{}

CMiniserverTableModel* Qt_MiniserverTableView::getMiniserverModel()
{
    return this->m_model;
}

//void Qt_MiniserverTableView::handleSelectionChange(const QItemSelection& selected, const QItemSelection& deselected) {
//}


void Qt_MiniserverTableView::handleConnectConfigClicked(const QModelIndex& index) {
        
    CConnectConfigButtonDelegate* button = qobject_cast<CConnectConfigButtonDelegate*>(sender());
    if (!button)
        return;

    if (!index.isValid())
        return;

    CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(this->model());
    if (!model) {
        return;
    }

    CMiniserver miniserver = model->m_searchText.isEmpty() ? model->miniserverlist->at(index.row()) : model->filteredMiniservers->at(index.row());
    int trueIndex = model->miniserverlist->indexOf(miniserver);


    emit  ConnectConfigClicked(index, miniserver);
}

void Qt_MiniserverTableView::insertRow(const CMiniserver& miniserver)
{
    m_model->insertRow(miniserver);
}


void Qt_MiniserverTableView::contextMenuEvent(QContextMenuEvent* event)
{
	QModelIndex clickedIndex = indexAt(event->pos());
	if (clickedIndex.isValid())
	{
		if (clickedIndex.column() == 1)
		{
			QMenu contextMenu;
			contextMenu.addAction("Edit Miniserver");
            contextMenu.addAction("Copy SNR");
			contextMenu.addAction("Copy Username");
			contextMenu.addAction("Copy Password");
			contextMenu.addAction("Extern WI");
			contextMenu.addAction("Intern WI");
			contextMenu.addAction("Deflog");
			contextMenu.addAction("FTP");
			contextMenu.addAction("LPH");
            contextMenu.addAction("LoxApp3.json");
			contextMenu.addAction("CrashLog Server");
            contextMenu.addAction("Reboot");
			contextMenu.addAction("Open in Loxone APP (BETA)");
			contextMenu.addAction("Download Prog Folder (BETA)");

			QAction* selectedItem = contextMenu.exec(event->globalPos());
			if (selectedItem)
			{
                CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(this->model());
                if (!model) {
                    return;
                }

				QString selectedItemText = selectedItem->text();
                CMiniserver miniserver = model->m_searchText.isEmpty() ? model->miniserverlist->at(clickedIndex.row()) : model->filteredMiniservers->at(clickedIndex.row());
                int trueIndex = model->miniserverlist->indexOf(miniserver);


                
				if (selectedItemText == "Edit Miniserver")
				{
                    CMiniserver newMiniserver = Qt_CreateEditMiniserver::createDialog("Edit Miniserver", &miniserver, model->miniserverlist);
                    model->miniserverlist->replace(trueIndex, newMiniserver);
                    if (!model->filteredMiniservers->empty())
                        model->filteredMiniservers->replace(clickedIndex.row(), newMiniserver);
                    model->printDebugDataChanged(model->index(clickedIndex.row(), clickedIndex.column()), newMiniserver);
                    model->dataChanged(model->index(clickedIndex.row(), 0), model->index(clickedIndex.column(), 8), {Qt::DisplayRole, Qt::EditRole});
                    clearSelection();
                    selectRow(clickedIndex.row());
				}
                else if (selectedItemText == "Copy SNR")
                {
                    QString username = QString::fromStdString(miniserver.getSerialNumber());
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(username);
                }
				else if (selectedItemText == "Copy Username")
				{
                    QString username = QString::fromStdString(miniserver.getAdminUser());
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(username);
				}
				else if (selectedItemText == "Copy Password")
				{
                    QString username = QString::fromStdString(miniserver.getAdminPassword());
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(username);
				}
                else if (selectedItemText == "Extern WI") {
                    QString link = QString("http://dns.loxonecloud.com/%1").arg(QString::fromStdString(miniserver.getSerialNumber()));
                    QDesktopServices::openUrl(QUrl(link));
                }
                else if (selectedItemText == "Intern WI") {
                    if (!miniserver.getLocalIP().empty()) {
                        QString link = QString("http://%1/").arg(QString::fromStdString(miniserver.getLocalIP()));
                    }
                }
                else if (selectedItemText == "Deflog") {
                    QString serialNumber = QString::fromStdString(miniserver.getSerialNumber());
                    QString localIP = QString::fromStdString(miniserver.getLocalIP());
                    QString username = QString::fromStdString(miniserver.getAdminUser());
                    QString password = QString::fromStdString(miniserver.getAdminPassword());
                    QString link = CSerialNumberHyperlinkDelegate::generateLink(serialNumber, localIP);
                    link += "/dev/fsget/log/def.log";
                    QUrl url = QUrl(link);

                    if (!localIP.isEmpty()) {
                        url.setUserName(username);
                        url.setPassword(password);
                       
                    }
                    else {
                        QString cloudlink = CWebService::getCloudDNSLink(miniserver);
                        cloudlink += "dev/fsget/log/def.log";
                        url.setUrl(cloudlink);
                        url.setUserName(username);
                        url.setPassword(password);
                    }
                    
                    QDesktopServices::openUrl(url);
                    
                }
                else if (selectedItemText == "FTP") {
                    
                    QString serialNumber = QString::fromStdString(miniserver.getSerialNumber());
                    QString localIP = QString::fromStdString(miniserver.getLocalIP());
                    QString username = QString::fromStdString(miniserver.getAdminUser());
                    QString password = QString::fromStdString(miniserver.getAdminPassword());
                    if (!localIP.isEmpty()) {
                        QUrl ftpUrl;
                        ftpUrl.setScheme("ftp");
                        ftpUrl.setUserName(username);
                        ftpUrl.setPassword(password);
                        ftpUrl.setHost(localIP);

                        QStringList arguments;
                        arguments << ftpUrl.toString();

                        QProcess::startDetached("explorer", arguments);
                        
                    }
                    else {
                        QMessageBox::warning(nullptr, "Warning", QString::fromStdString(MyConstants::Strings::MessageBox_FTP_Local_IP_not_defined));
                    }
                    
                    
                }
                else if (selectedItemText == "LPH") {
                    QString serialNumber = QString::fromStdString(miniserver.getSerialNumber());
                    QString link = QString::fromStdString(MyConstants::Strings::Link_LPH) + serialNumber;
                    QDesktopServices::openUrl(QUrl(link));
                }
                else if (selectedItemText == "CrashLog Server") {
                    QString serialNumber = QString::fromStdString(miniserver.getSerialNumber());
                    QString link = QString::fromStdString(MyConstants::Strings::Link_CrashLog_Server) + serialNumber;
                    QDesktopServices::openUrl(QUrl(link));
                }
                else if (selectedItemText == "Open in Loxone APP (BETA)") {

                    openInLoxoneApp(miniserver);
                }
                else if (selectedItemText == "Download Prog Folder (BETA)") {
                    emit downloadProgFolderPressed(miniserver);
                }
                else if (selectedItemText == "Reboot") {
                    QString serialNumber = QString::fromStdString(miniserver.getSerialNumber());
                    QString localIP = QString::fromStdString(miniserver.getLocalIP());
                    QString link = CSerialNumberHyperlinkDelegate::generateLink(serialNumber, localIP);
                    link += "/dev/sys/reboot";
                    QUrl url = QUrl(link);

                    if (localIP.isEmpty()) {
                        QString cloudlink = CWebService::getCloudDNSLink(miniserver);
                        cloudlink += "dev/sys/reboot";
                        url.setUrl(cloudlink);
                    }

                    QDesktopServices::openUrl(url);
                }
                else if (selectedItemText == "LoxApp3.json") {
                    QString serialNumber = QString::fromStdString(miniserver.getSerialNumber());
                    QString localIP = QString::fromStdString(miniserver.getLocalIP());
                    QString username = QString::fromStdString(miniserver.getAdminUser());
                    QString password = QString::fromStdString(miniserver.getAdminPassword());
                    QString link = CSerialNumberHyperlinkDelegate::generateLink(serialNumber, localIP);
                    link += "/data/LoxAPP3.json";
                    QUrl url = QUrl(link);

                    if (!localIP.isEmpty()) {
                        url.setUserName(username);
                        url.setPassword(password);

                    }
                    else {
                        QString cloudlink = CWebService::getCloudDNSLink(miniserver);
                        cloudlink += "data/LoxAPP3.json";
                        url.setUrl(cloudlink);
                        url.setUserName(username);
                        url.setPassword(password);
                    }

                    QDesktopServices::openUrl(url);
                }
				
			}
		}
        else if (clickedIndex.column() == 3) {
            QMenu contextMenu;
            contextMenu.addAction("Copy Local IP");
            //contextMenu.addAction("Get Local IP from Miniserver");
            contextMenu.addAction("Use Local IP in column");
            

            QAction* selectedItem = contextMenu.exec(event->globalPos());
            if (selectedItem)
            {
                CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(this->model());
                if (!model) {
                    return;
                }
                CMiniserver miniserver = model->m_searchText.isEmpty() ? model->miniserverlist->at(clickedIndex.row()) : model->filteredMiniservers->at(clickedIndex.row());
                int trueIndex = model->miniserverlist->indexOf(miniserver);
                QString selectedItemText = selectedItem->text();
                
                if (selectedItemText == "Copy Local IP") {
                    QString project = QString::fromStdString(miniserver.getMiniserverProject());
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(CMiniserver::getLocalIPfromListviewProjectText(project));
                }
                //else if (selectedItemText == "Get Local IP from Miniserver") {
                //
                //}
                else if (selectedItemText == "Use Local IP in column") {
                    QString project = QString::fromStdString(miniserver.getMiniserverProject());
                    miniserver.setLocalIP(CMiniserver::getLocalIPfromListviewProjectText(project).toStdString());
                    model->miniserverlist->replace(trueIndex, miniserver);
                    if (!model->filteredMiniservers->empty())
                        model->filteredMiniservers->replace(clickedIndex.row(), miniserver);
                    model->printDebugDataChanged(model->index(clickedIndex.row(), clickedIndex.column()), miniserver);
                    model->dataChanged(model->index(clickedIndex.row(), 0), model->index(clickedIndex.column(), 8), { Qt::DisplayRole, Qt::EditRole });

                }
            }
        }
        else if (clickedIndex.column() == 5) {
            QMenu contextMenu;
            contextMenu.addAction("Set Release");
            contextMenu.addAction("Set Alpha");
            contextMenu.addAction("Set Beta");
            contextMenu.addAction("AutoUpdate");
            contextMenu.addAction("AutoUpdate all selected (BETA)");

            QAction* selectedItem = contextMenu.exec(event->globalPos());
            if (selectedItem) {
                CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(this->model());
                if (!model) {
                    return;
                }

                QString selectedItemText = selectedItem->text();
                CMiniserver miniserver = model->m_searchText.isEmpty() ? model->miniserverlist->at(clickedIndex.row()) : model->filteredMiniservers->at(clickedIndex.row());
                int trueIndex = model->miniserverlist->indexOf(miniserver);
                bool bLocal = !miniserver.getLocalIP().empty();
                QString webService, updateLevel;
                QString interestedValue = "value";

                if (selectedItemText == "Set Release") {
                    webService = "dev/cfg/updatelevel/release";
                    
                }
                else if (selectedItemText == "Set Alpha") {
                    webService = "dev/cfg/updatelevel/alpha";
                }
                else if (selectedItemText == "Set Beta") {
                    webService = "dev/cfg/updatelevel/beta";
                }
                else if (selectedItemText == "AutoUpdate") {
                    webService = "/dev/sys/autoupdate";
                    interestedValue = "Code";
                    autoUpdateSingleMiniserver(bLocal, miniserver, webService, interestedValue, trueIndex);
                    return;
                }

                if (bLocal && selectedItemText != "AutoUpdate" && selectedItemText != "AutoUpdate all selected (BETA)") {
                    CWebService::sendCommandRest_Version_Local_Gen1(miniserver, webService, interestedValue);
                    updateLevel = CWebService::sendCommandRest_Version_Local_Gen1(miniserver, "dev/cfg/updatelevel", "value");
                    if (updateLevel != "error") {
                        miniserver.setUpdatelevel(updateLevel.toStdString());
                    }
                    else {
                        miniserver.setUpdatelevel(updateLevel.toStdString());
                        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_retreiving_information_timeout);
                    }
                    
                    model->miniserverlist->replace(trueIndex, miniserver);
                    if (!model->filteredMiniservers->empty())
                        model->filteredMiniservers->replace(clickedIndex.row(), miniserver);
                    model->printDebugDataChanged(model->index(clickedIndex.row(), clickedIndex.column()), miniserver);
                    model->dataChanged(model->index(clickedIndex.row(), 0), model->index(clickedIndex.column(), 8), { Qt::DisplayRole, Qt::EditRole });
                    return;
                }
                else if (!bLocal && selectedItemText != "AutoUpdate" && selectedItemText != "AutoUpdate all selected (BETA)") {
                    QString url = CWebService::getCloudDNSLink(miniserver);
                    CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, webService, interestedValue,url);
                    updateLevel = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, "dev/cfg/updatelevel", "value", url);
                    if (updateLevel != "error") {
                        miniserver.setUpdatelevel(updateLevel.toStdString());
                    }
                    else {
                        miniserver.setUpdatelevel(updateLevel.toStdString());
                        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_retreiving_information_timeout);
                    }
                    model->miniserverlist->replace(trueIndex, miniserver);
                    if (!model->filteredMiniservers->empty())
                        model->filteredMiniservers->replace(clickedIndex.row(), miniserver);
                    model->printDebugDataChanged(model->index(clickedIndex.row(), clickedIndex.column()), miniserver);
                    model->dataChanged(model->index(clickedIndex.row(), 0), model->index(clickedIndex.column(), 8), { Qt::DisplayRole, Qt::EditRole });
                    return;
                }


                
                if (selectedItemText == "AutoUpdate all selected (BETA)") {
                    QModelIndexList selectedIndexes = selectionModel()->selectedRows();
                    std::sort(selectedIndexes.begin(), selectedIndexes.end(), [](const QModelIndex& a, const QModelIndex& b) { return a.row() > b.row(); });
                    webService = "/dev/sys/autoupdate";
                    interestedValue = "Code";
                    qDebug() << selectedIndexes;

                    for (QModelIndex index : selectedIndexes) {
                        miniserver = model->m_searchText.isEmpty() ? model->miniserverlist->at(clickedIndex.row()) : model->filteredMiniservers->at(clickedIndex.row());
                        int trueIndex = model->miniserverlist->indexOf(miniserver);
                        autoUpdateSingleMiniserver(bLocal, miniserver, webService, interestedValue, trueIndex);
                    }
                }

                //error if failed
                
                
            }
        }

        else if (clickedIndex.column() == 7) {
            QMenu contextMenu;
            contextMenu.addAction("Remove all local IPs");
            
            QAction* selectedItem = contextMenu.exec(event->globalPos());
            if (selectedItem)
            {
                CMiniserverTableModel* model = qobject_cast<CMiniserverTableModel*>(this->model());
                if (!model) {
                    return;
                }

                QString selectedItemText = selectedItem->text();

                if (selectedItemText == "Remove all local IPs") {
                    int maxCount = model->m_searchText.isEmpty() ? model->miniserverlist->count() : model->filteredMiniservers->count();
                    for (int i = 0; i < maxCount; i++) {
                        CMiniserver miniserver = model->m_searchText.isEmpty() ? model->miniserverlist->at(i) : model->filteredMiniservers->at(i);
                        int trueIndex = model->miniserverlist->indexOf(miniserver);
                        miniserver.setLocalIP("");
                        model->miniserverlist->replace(trueIndex, miniserver);
                        if (!model->filteredMiniservers->empty())
                            model->filteredMiniservers->replace(i, miniserver);
                        model->printDebugDataChanged(model->index(i, 7), miniserver);
                        model->dataChanged(model->index(i, 0), model->index(clickedIndex.column(), 8), { Qt::DisplayRole, Qt::EditRole });
                    }
                }
            }
        }

		else
		{
			QTableView::contextMenuEvent(event);
		}
	}
}

void Qt_MiniserverTableView::autoUpdateSingleMiniserver(bool bLocal, CMiniserver& miniserver, QString& webService, QString& interestedValue, int index)
{
    QString ret;
    if (bLocal) {
        ret = CWebService::sendCommandRest_Version_Local_Gen1(miniserver, webService, interestedValue);

    }
    else {
        ret = CWebService::sendCommandRest_Version_Remote_Cloud(miniserver, webService, interestedValue,"");
    }
    if (ret == QString::fromStdString(MyConstants::Strings::WebService_Success_Code)) {
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_AutoUpdate);
        m_model->miniserverlist->replace(index, miniserver);
    }
    else {
        miniserver.setMiniserverStatus(MyConstants::Strings::Listview_MS_Status_retreiving_information_timeout);
        m_model->miniserverlist->replace(index, miniserver);
    }
}

void Qt_MiniserverTableView::openInLoxoneApp(CMiniserver& miniserver)
{
    QString link; 

    if (miniserver.getLocalIP().empty()) {
        link = QString("loxone://ms?host=%1&usr=%2&pwd=%3")
            .arg(QUrl::toPercentEncoding("dns.loxonecloud.com/" + QString::fromStdString(miniserver.getSerialNumber())),
                QUrl::toPercentEncoding(QString::fromStdString(miniserver.getAdminUser())),
                QUrl::toPercentEncoding(QString::fromStdString(miniserver.getAdminPassword())));
    }
    else {
        link = QString("loxone://ms?host=%1&usr=%2&pwd=%3")
            .arg(QUrl::toPercentEncoding(QString::fromStdString(miniserver.getLocalIP())),
                QUrl::toPercentEncoding(QString::fromStdString(miniserver.getAdminUser())),
                QUrl::toPercentEncoding(QString::fromStdString(miniserver.getAdminPassword())));
    }

    if (QDesktopServices::openUrl(QUrl(link))) {
        qDebug() << "Link opened successfully.";
    }
    else {
        qDebug() << "Failed to open the link.";
    }

}

//void Qt_MiniserverTableView::keyPressEvent(QKeyEvent* event)
//{
//    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down
//        || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
//    {
//        // Get the current selected index
//
//        int currentColumn = currentIndex().column();
//        
//        if (event->key() == Qt::Key_Left)
//        {
//            currentColumn--;
//        }
//        else if (event->key() == Qt::Key_Right)
//        {
//            currentColumn++;
//        }
//
//        //QModelIndex newIndex = model()->index(currentIndex().row(), currentColumn);
//        //if (newIndex.isValid())
//        //{
//        //    setCurrentIndex(newIndex);
//        //}
//
//        // Check if it's the specific row where you don't want to activate editing
//        if (currentColumn == 6)
//        {
//            event->ignore(); // Ignore the arrow key event to prevent editing
//            return;
//        }
//
//    }
//
//    // If not in the specific row or for other keys, allow normal key handling
//    QTableView::keyPressEvent(event);
//
//}

//bool Qt_MiniserverTableView::eventFilter(QObject* object, QEvent* event)
//{
//    if (object == viewport())
//    {
//        if (event->type() == QEvent::MouseMove)
//        {
//            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
//            QModelIndex index = indexAt(mouseEvent->pos());
//            if (index.isValid() && index.column() == 1) {
//                setLinkCursor(true);
//            }
//            else {
//                setLinkCursor(false);
//            }
//                
//        }
//        else if (event->type() == QEvent::Leave)
//        {
//            setLinkCursor(false);
//        }
//    }
//
//    return QTableView::eventFilter(object, event);
//}


void Qt_MiniserverTableView::setLinkCursor(bool enabled)
{
    if (enabled) {
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
    }
    else {
        QApplication::restoreOverrideCursor();
    }
       
}

void Qt_MiniserverTableView::setEnabledTableView(bool state)
{
    setEnabled(state);
    emit enabledStateChanged(state);
}

void Qt_MiniserverTableView::handleSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    emit mySelectionChanged(selected, deselected);
    qDebug() << "selection changed: " + QString::number(selectionModel()->selectedRows().count());

}

