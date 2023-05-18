#pragma once

#include "stdafx.h" 
#include <QThread>
#include "Qt_MiniserverTableView.h"
#include "Qt_Bottom_Action_Buttons.h"
#include "Qt_Statusbar.h"
#include "CMiniserver.h"

class CConnectConfigWorker  : public QThread
{
	Q_OBJECT

public:
	CConnectConfigWorker(QObject *parent);
	CConnectConfigWorker(QObject* parent, Qt_MiniserverTableView* tableview, Qt_Bottom_Action_Buttons* actionButtons, Qt_Statusbar* bottomStatus);
	~CConnectConfigWorker();

	void setMiniserver(const CMiniserver ms);

private:

	Qt_MiniserverTableView* tableViewMiniserver;
	Qt_Bottom_Action_Buttons* bottom_buttons;
	Qt_Statusbar* statusbar;
	CMiniserver miniserver;

protected:
	void run() override;


signals:
	void connectingCompleted();
	void connectingCanceled();


};
