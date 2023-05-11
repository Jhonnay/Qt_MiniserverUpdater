#pragma once

#include "stdafx.h"
#include <QMainWindow>
#include "ui_Qt_Bottom_Action_Buttons.h"

class Qt_Bottom_Action_Buttons : public QWidget
{
	Q_OBJECT

public:
	explicit Qt_Bottom_Action_Buttons(QWidget* parent = nullptr);
	~Qt_Bottom_Action_Buttons();

	void onButtonUpdateClicked();

	void onButtonRefreshClicked();

	void onButtonRemoveClicked();

	void onButtonAddClicked();

	void onButtonCancelClicked();

	void onCheckBoxHandsfreeClicked();

	void setDisabledAllExceptCancel(bool setting);

signals:
	void buttonUpdateClicked();
	void buttonRefreshClicked();
	void buttonRemoveClicked();
	void buttonAddClicked();
	void buttonCancelClicked();
	void checkBoxHandsfreeClicked();


private:
	QHBoxLayout* hBox;
	QPushButton* btnUpdate;
	QPushButton* btnRefresh;
	QPushButton* btnRemove;
	QPushButton* btnAdd;
	QPushButton* btnCancel;
	QCheckBox* checkbox_handsfreemode;
	Ui::Qt_Bottom_Action_ButtonsClass ui;
};
