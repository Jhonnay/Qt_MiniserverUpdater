#include "CComboBoxLanguageDelegate.h"

CComboBoxLanguageDelegate::CComboBoxLanguageDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{}

CComboBoxLanguageDelegate::~CComboBoxLanguageDelegate()
{}



QWidget* CComboBoxLanguageDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option)
        Q_UNUSED(index)

        QComboBox* comboBox = new QComboBox(parent);
    comboBox->addItems(CConfig::LanguageList);
    comboBox->setEditable(false);

    return comboBox;
}

void CComboBoxLanguageDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    if (comboBox)
    {
        int idx = comboBox->findText(index.data(Qt::EditRole).toString());
        comboBox->setCurrentIndex(idx);
    }
}

void CComboBoxLanguageDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    if (comboBox)
    {
        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
}