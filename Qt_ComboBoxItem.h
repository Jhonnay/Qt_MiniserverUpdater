#pragma once

#include "stdafx.h"
#include <QWidget>
#include "ui_Qt_ComboBoxItem.h"
#include "CConfig.h"

class Qt_ComboBoxItem : public QComboBox
{
	Q_OBJECT

public:
	Qt_ComboBoxItem(QWidget *parent = nullptr);
	Qt_ComboBoxItem(QTreeWidgetItem*, int);
	~Qt_ComboBoxItem();

public slots:
	void changeItem(int);


private:
	QTreeWidgetItem* item = NULL;
	int column = NULL;
	//Ui::Qt_ComboBoxItemClass ui;
};
