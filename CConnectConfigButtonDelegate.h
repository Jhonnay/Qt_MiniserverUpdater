#pragma once
#include "stdafx.h"
#include <QStyledItemDelegate>

class CConnectConfigButtonDelegate  : public QStyledItemDelegate
{
	Q_OBJECT

public:
	CConnectConfigButtonDelegate(QObject *parent);
	~CConnectConfigButtonDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

signals:
	void clicked(const QModelIndex& index) const;
};


