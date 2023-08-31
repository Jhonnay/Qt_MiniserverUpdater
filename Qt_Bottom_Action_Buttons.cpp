#include "stdafx.h"
#include "Qt_Bottom_Action_Buttons.h"
#include <iostream>
#include <Windows.h>


Qt_Bottom_Action_Buttons::Qt_Bottom_Action_Buttons(QWidget* parent)
	: QWidget(parent)
{
	btnUpdate = new QPushButton(QString::fromStdString("Update"), this);
	btnRefresh = new QPushButton(QString::fromStdString("🔄"), this);
	btnAdd = new QPushButton(QString::fromStdString("➕"), this);
	btnCancel = new QPushButton(QString::fromStdString("Cancel"), this);
	btnRemove = new QPushButton(QString::fromStdString("➖"), this);
	//checkbox_handsfreemode = new QCheckBox(QString::fromStdString("Handsfree Update Mode"), this);
	//checkbox_handsfreemode->setChecked(true);


	hBox = new QHBoxLayout;
	hBox->addWidget(btnUpdate);
	hBox->addWidget(btnRefresh);
	hBox->addWidget(btnRemove);
	hBox->addWidget(btnAdd);
	hBox->addWidget(btnCancel);
	//hBox->addWidget(checkbox_handsfreemode);
	hBox->setContentsMargins(8, 8, 200, 8);

	setLayout(hBox);

	connect(btnUpdate, &QPushButton::clicked, this, &Qt_Bottom_Action_Buttons::onButtonUpdateClicked);
	connect(btnRefresh, &QPushButton::clicked, this, &Qt_Bottom_Action_Buttons::onButtonRefreshClicked);
	connect(btnRemove, &QPushButton::clicked, this, &Qt_Bottom_Action_Buttons::onButtonRemoveClicked);
	connect(btnAdd, &QPushButton::clicked, this, &Qt_Bottom_Action_Buttons::onButtonAddClicked);
	connect(btnCancel, &QPushButton::clicked, this, &Qt_Bottom_Action_Buttons::onButtonCancelClicked);
	//connect(checkbox_handsfreemode, &QCheckBox::clicked, this, &Qt_Bottom_Action_Buttons::onCheckBoxHandsfreeClicked);
}




Qt_Bottom_Action_Buttons::~Qt_Bottom_Action_Buttons()
{
	delete hBox;
	delete btnUpdate;
	delete btnRefresh;
	delete btnRemove;
	delete btnAdd;
	delete btnCancel;
	//delete checkbox_handsfreemode;
}

void Qt_Bottom_Action_Buttons::onButtonUpdateClicked()
{
	qDebug() << "Update Button pressed!";

	emit buttonUpdateClicked();
	//btnUpdate->setDisabled(true);
}

void Qt_Bottom_Action_Buttons::onButtonRefreshClicked()
{
	emit buttonRefreshClicked();
}

void Qt_Bottom_Action_Buttons::onButtonRemoveClicked()
{
	emit buttonRemoveClicked();
}

void Qt_Bottom_Action_Buttons::onButtonAddClicked()
{
	emit buttonAddClicked();
}

void Qt_Bottom_Action_Buttons::onButtonCancelClicked()
{
	emit buttonCancelClicked();
}

void Qt_Bottom_Action_Buttons::onCheckBoxHandsfreeClicked()
{
	emit checkBoxHandsfreeClicked();
}

void Qt_Bottom_Action_Buttons::setDisabledAllExceptCancelAdd(bool setting)
{
	this->btnRefresh->setDisabled(setting);
	this->btnRemove->setDisabled(setting);
	this->btnUpdate->setDisabled(setting);
}

void Qt_Bottom_Action_Buttons::setDisabledAddMiniserverButton(bool setting) 
{
	this->btnAdd->setDisabled(setting);
}

//bool Qt_Bottom_Action_Buttons::isHandsfreeModeEnabled()
//{
//	return checkbox_handsfreemode->isChecked();
//}
