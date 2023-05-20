#pragma once

#include "stdafx.h"
#include "CMiniserverTableModel.h"
#include <QDialog>

class Qt_CreateEditMiniserver : public QDialog
{
	Q_OBJECT

public:
	Qt_CreateEditMiniserver(const QString& title = QString(), QWidget* parent = nullptr);
	~Qt_CreateEditMiniserver();

	static CMiniserver createDialog(const QString& title = QString(), CMiniserver* miniserver = nullptr, QList<CMiniserver>* list =nullptr);

private:
	//Ui::Qt_CreateEditMiniserverClass ui;
	CMiniserver miniserver;
	QLineEdit* serialNumberLineEdit = new QLineEdit();
	QLineEdit* adminUserLineEdit = new QLineEdit();
	QLineEdit* adminPasswordLineEdit = new QLineEdit();
	QLineEdit* localIPLineEdit = new QLineEdit;

	QFormLayout* formLayout = new QFormLayout;
	QLabel* labelHint = new QLabel("* necessary fields");
	QPushButton* okButton = new QPushButton("OK");
	QPushButton* cancelButton = new QPushButton("Cancel");

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	QVBoxLayout* mainLayout = new QVBoxLayout;

	CMiniserver getMiniserver();
};

