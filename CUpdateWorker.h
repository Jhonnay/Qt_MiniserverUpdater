#pragma once

#include "stdafx.h"
#include "Qt_Bottom_Action_Buttons.h"
#include "Qt_MiniserverTableView.h"
#include "Qt_Statusbar.h"
#include <QThread>


class CUpdateWorker  : public QThread
{
	Q_OBJECT

public:
	CUpdateWorker(QObject *parent);
	CUpdateWorker(QObject* parent, Qt_MiniserverTableView* tableview, Qt_Bottom_Action_Buttons* actionButtons, Qt_Statusbar* bottomStatus);
	~CUpdateWorker();

private:

	Qt_MiniserverTableView* tableViewMiniserver;
	Qt_Bottom_Action_Buttons* bottom_buttons;
	Qt_Statusbar* statusbar;

protected:
	void run() override;


signals:
	void updatingCompleted();
	void updatingCanceled();
	void updateStatusBarProgress(int progress, QString progresstext);

};
