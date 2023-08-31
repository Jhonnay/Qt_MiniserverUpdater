#include "CConnectConfigButtonDelegate.h"

CConnectConfigButtonDelegate::CConnectConfigButtonDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{}

CConnectConfigButtonDelegate::~CConnectConfigButtonDelegate()
{}




QWidget* CConnectConfigButtonDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
   

    QPushButton* button = new QPushButton(parent);
    button->setText("Connect");
    button->setProperty("indexRow", index.row());
    button->setFlat(true);
    button->setCheckable(false);
    

    //emit directly. otherwise 2 clicks are necessary. 
    emit clicked(index);
    //connecting in order to allow pressing the button when cell is already in editing mode
    //not working
    connect(button, &QPushButton::pressed, [this, index]() {
        emit clicked(index);
        });
       

    return button;
}

void CConnectConfigButtonDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(index)

    
}

void CConnectConfigButtonDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    Q_UNUSED(editor)
        Q_UNUSED(model)
        Q_UNUSED(index)
}

void CConnectConfigButtonDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void CConnectConfigButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);

    // Draw the background
    QStyleOptionButton buttonOption;
    buttonOption.rect = option.rect;
    buttonOption.palette = option.palette;
    buttonOption.state = QStyle::State_Enabled | QStyle::State_Raised;

    QApplication::style()->drawControl(QStyle::CE_PushButtonBevel, &buttonOption, painter);
    QApplication::style()->drawItemText(painter, option.rect, Qt::AlignCenter, option.palette, false, "Connect");
}
