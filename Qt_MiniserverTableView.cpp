#include "Qt_MiniserverTableView.h"
#include "CSerialNumberHyperlinkDelegate.h"
#include "CWebService.h"
#include "MyConstants.h"
#include "Qt_CreateEditMiniserver.h"


Qt_MiniserverTableView::Qt_MiniserverTableView(QWidget *parent)
	: QTableView(parent)
{
    Qt_MiniserverTableView(NULL, this); //does not work
}



Qt_MiniserverTableView::Qt_MiniserverTableView(QList<CMiniserver>* miniservers, QWidget* parent)
    : QTableView(parent), m_model(new CMiniserverTableModel(miniservers, this))
{
    setModel(m_model);
    setSelectionMode(QAbstractItemView::MultiSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setMinimumWidth(35);
    setSortingEnabled(true);
    setMouseTracking(true);
    //setUpdatesEnabled(true);
    setEditTriggers(QAbstractItemView::AllEditTriggers); //enable single click for edit mode
    //setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::EditKeyPressed);
    horizontalHeader()->setSectionsMovable(true);     //set Header moveable
    //connect(horizontalHeader(), &QHeaderView::sectionClicked, m_model, &CMiniserverTableModel::sort);
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

    
}


Qt_MiniserverTableView::~Qt_MiniserverTableView()
{}

CMiniserverTableModel* Qt_MiniserverTableView::getMiniserverModel()
{
    return this->m_model;
}


void Qt_MiniserverTableView::handleConnectConfigClicked(const QModelIndex& index) {
    int row = index.row();
    CConnectConfigButtonDelegate* button = qobject_cast<CConnectConfigButtonDelegate*>(sender());
    if (!button)
        return;

    //QModelIndex _index = indexAt(button);
    if (!index.isValid())
        return;

    const CMiniserver& miniserver = m_model->miniserverlist->at(index.row());
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
			contextMenu.addAction("CrashLog Server");
			contextMenu.addAction("Open in Loxone APP (BETA)");
			contextMenu.addAction("Download Prog Folder (BETA)");

			QAction* selectedItem = contextMenu.exec(event->globalPos());
			if (selectedItem)
			{
				QString selectedItemText = selectedItem->text();
                CMiniserver miniserver = m_model->miniserverlist->at(clickedIndex.row());
				if (selectedItemText == "Edit Miniserver")
				{
                    CMiniserver newMiniserver = Qt_CreateEditMiniserver::createDialog("Edit Miniserver", &miniserver, m_model->miniserverlist);
                    m_model->miniserverlist->replace(clickedIndex.row(), newMiniserver);
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

                }
                else if (selectedItemText == "Download Prog Folder (BETA)") {
                    CWebService::DownloadProgFolder(miniserver);
                }
				
			}
		}
        else if (clickedIndex.column() == 3) {
            QMenu contextMenu;
            contextMenu.addAction("Copy Local IP");
            contextMenu.addAction("Get Local IP from Miniserver");
            contextMenu.addAction("Use Local IP in column");
            

            QAction* selectedItem = contextMenu.exec(event->globalPos());
            if (selectedItem)
            {
                QString selectedItemText = selectedItem->text();
                CMiniserver miniserver = m_model->miniserverlist->at(clickedIndex.row());

                if (selectedItemText == "Copy Local IP") {
                    QString project = QString::fromStdString(miniserver.getMiniserverProject());
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(CMiniserver::getLocalIPfromListviewProjectText(project));
                }
                else if (selectedItemText == "Get Local IP from Miniserver") {

                }
                else if (selectedItemText == "Use Local IP in column") {
                    QString project = QString::fromStdString(miniserver.getMiniserverProject());
                    miniserver.setLocalIP(CMiniserver::getLocalIPfromListviewProjectText(project).toStdString());
                    m_model->miniserverlist->replace(clickedIndex.row(), miniserver);

                }
            }
        }

		else
		{
			QTableView::contextMenuEvent(event);
		}
	}
}

bool Qt_MiniserverTableView::eventFilter(QObject* object, QEvent* event)
{
    if (object == viewport())
    {
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QModelIndex index = indexAt(mouseEvent->pos());
            if (index.isValid() && index.column() == 1) {
                setLinkCursor(true);
            }
            else {
                setLinkCursor(false);
            }
                
        }
        else if (event->type() == QEvent::Leave)
        {
            setLinkCursor(false);
        }
    }

    return QTableView::eventFilter(object, event);
}


void Qt_MiniserverTableView::setLinkCursor(bool enabled)
{
    if (enabled) {
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
    }
    else {
        QApplication::restoreOverrideCursor();
    }
       
}