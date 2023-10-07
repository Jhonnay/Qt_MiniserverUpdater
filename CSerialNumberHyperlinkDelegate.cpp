#include "CSerialNumberHyperlinkDelegate.h"
#include "CMiniserver.h"
#include "CMiniserverTableModel.h"
#include "CWebService.h"

CSerialNumberHyperlinkDelegate::CSerialNumberHyperlinkDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{

}

CSerialNumberHyperlinkDelegate::~CSerialNumberHyperlinkDelegate()
{}


void CSerialNumberHyperlinkDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == 1) {
        QString serialNumber = index.data().toString(); // Retrieve the serial number from the model data
        QString displayText = QString(serialNumber);

        // Render the rich text link
        QStyleOptionViewItem updatedOption = option;
        updatedOption.text = displayText;
        updatedOption.palette.setColor(QPalette::Text, Qt::blue);
        updatedOption.font.setUnderline(true);
        //QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &updatedOption, painter);
        QStyledItemDelegate::paint(painter, updatedOption, index); // Let the delegate handle text rendering
    }
    else {
        QStyledItemDelegate::paint(painter, option, index);
    }

}

QSize CSerialNumberHyperlinkDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == 1) {
        QString serialNumber = index.data().toString();
        QString displayText = QString(serialNumber);
        // Calculate the size hint based on the rich text link
        QSize hint = QStyledItemDelegate::sizeHint(option, index);
        QFontMetrics fontMetrics(option.font);
        QSize textSize = fontMetrics.size(Qt::TextSingleLine, displayText);
        // Adjust the width to ensure the text fits completely
        hint.setWidth(qMax(hint.width(), textSize.width()));
        return hint;
    }

    return QStyledItemDelegate::sizeHint(option, index);
}

bool CSerialNumberHyperlinkDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
    if (event->type() == QEvent::MouseButtonRelease && index.column() == 1) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            CMiniserverTableModel* miniserverModel = dynamic_cast<CMiniserverTableModel*>(model);
            if (miniserverModel)
            {
                                             
                CMiniserver miniserver = miniserverModel->m_searchText.isEmpty() ? miniserverModel->miniserverlist->at(index.row()) : miniserverModel->filteredMiniservers->at(index.row());
                QString serialNumber = QString::fromStdString(miniserver.getSerialNumber());
                QString localIP = QString::fromStdString(miniserver.getLocalIP());
                QString username = QString::fromStdString(miniserver.getAdminUser());
                QString password = QString::fromStdString(miniserver.getAdminPassword());
                QString link = generateLink(serialNumber, localIP);

                //QUrl url(link);
                //if (!localIP.isEmpty())
                //{
                //    url.setUserName(username);
                //    url.setPassword(password);
                //}
                //else
                //{
                //    QString cloudlink = CWebService::getCloudDNSLink(miniserver);
                //    //cloudlink += "dev/fsget/log/def.log";
                //    url.setUrl(cloudlink);
                //    url.setUserName(username);
                //    url.setPassword(password);
                //}
                //QString urlString = url.toString();
                QDesktopServices::openUrl(QUrl(link));
            }

            return true;
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);

}

QString CSerialNumberHyperlinkDelegate::generateLink(const QString& serialNumber, const QString& localIP) {
    // Generate the appropriate link based on the local IP
    if (!localIP.isEmpty()) {
        // Use local IP link
        return QString("http://%1").arg(localIP);
    }
    else {
        // Use default link
        return QString("http://dns.loxonecloud.com/%1").arg(serialNumber);
    }
}


void CSerialNumberHyperlinkDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}