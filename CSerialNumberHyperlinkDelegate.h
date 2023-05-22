#pragma once
#include "stdafx.h"
#include <QStyledItemDelegate>

class CSerialNumberHyperlinkDelegate  : public QStyledItemDelegate
{
	Q_OBJECT

public:
	CSerialNumberHyperlinkDelegate(QObject *parent = nullptr);
	~CSerialNumberHyperlinkDelegate();
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
	static QString generateLink(const QString& serialNumber, const QString& localIP);
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
