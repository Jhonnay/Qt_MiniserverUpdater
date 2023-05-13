#pragma once
#include "stdafx.h"
#include "CConfig.h"
#include <QStyledItemDelegate>


class CComboBoxLanguageDelegate  : public QStyledItemDelegate
{
	Q_OBJECT

public:
	CComboBoxLanguageDelegate(QObject *parent);
	~CComboBoxLanguageDelegate();

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

};
