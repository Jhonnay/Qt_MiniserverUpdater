#include "Qt_ComboBoxItem.h"

Qt_ComboBoxItem::Qt_ComboBoxItem(QWidget *parent)
	: QComboBox(parent)
{
	this->setMaximumWidth(50);
	//ui.setupUi(this);
}

Qt_ComboBoxItem::Qt_ComboBoxItem(QTreeWidgetItem* item, int column)
{
	this->item = item;
	this->column = column;
	this->setMaximumWidth(50);
	connect(this, SIGNAL(currentIndexChanged(int)), SLOT(changeItem(int)));
	this->addItems(CConfig::LanguageList);
}

Qt_ComboBoxItem::~Qt_ComboBoxItem()
{}

//https://stackoverflow.com/questions/1667688/qcombobox-inside-qtreewidgetitem
void Qt_ComboBoxItem::changeItem(int index) {
	if (index >= 0)
	{
		item->setData(this->column, Qt::UserRole, this->itemText(index));
		qDebug() << item->data(this->column, Qt::UserRole).toString();

	}
}