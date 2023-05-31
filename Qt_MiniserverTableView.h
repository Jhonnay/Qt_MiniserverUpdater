#pragma once

#include "stdafx.h"
#include <QTableView>
#include "CMiniserver.h"
#include "CMiniserverTableModel.h"
#include "CComboBoxLanguageDelegate.h"
#include "CConnectConfigButtonDelegate.h"

class Qt_MiniserverTableView : public QTableView
{
	Q_OBJECT

public:
	Qt_MiniserverTableView(QWidget *parent = nullptr);
	explicit Qt_MiniserverTableView(QList<CMiniserver>* miniservers, QWidget* parent = nullptr);
	~Qt_MiniserverTableView();
	CMiniserverTableModel* getMiniserverModel();
	

protected: 
	void contextMenuEvent(QContextMenuEvent* event) override;
	void autoUpdateSingleMiniserver(bool bLocal, CMiniserver& miniserver, QString& webService, QString& interestedValue, QModelIndex& clickedIndex);
	bool eventFilter(QObject* object, QEvent* event) override;

public slots:
	void update() { QTableView::update(); }
	void repaint() { QTableView::repaint(); }
	void insertRow(const CMiniserver& miniserver);

private slots: 
	void handleConnectConfigClicked(const QModelIndex& index);
	

	
signals:
	void ConnectConfigClicked(const QModelIndex& index, const CMiniserver& miniserver);

private:
	CMiniserverTableModel* m_model;
	void setLinkCursor(bool enabled);

};
