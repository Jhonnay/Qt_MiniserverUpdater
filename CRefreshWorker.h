#pragma once

#include "stdafx.h"
#include "Qt_Bottom_Action_Buttons.h"
#include "Qt_MiniserverTableView.h"
#include "Qt_Statusbar.h"
#include <QThread>

class CRefreshWorker  : public QThread
{
	Q_OBJECT

public:
	CRefreshWorker(QObject* parent, Qt_MiniserverTableView* tableview, Qt_Bottom_Action_Buttons* actionButtons, Qt_Statusbar* bottomStatus);
	~CRefreshWorker();

private:
	Qt_MiniserverTableView* tableViewMiniserver;
	Qt_Bottom_Action_Buttons* bottom_buttons;
	Qt_Statusbar* statusbar;


protected:
	void run() override;


signals:
	void refreshCompleted();
	void refreshCanceled();
	void updateStatusBarProgress(int progress, QString progresstext);
	void setEnableTableview(bool state);

};
