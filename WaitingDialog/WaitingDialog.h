#pragma once

#include <QtWidgets/qdialog.h>
#include <QtGui/qmovie.h>
#include "ui_WaitingDialog.h"

class WaitingDialog : public QDialog
{
	Q_OBJECT

public:
	WaitingDialog(QWidget* parent = nullptr, const bool& enableProgressBar = false,
		const bool& enableTips = true);
	~WaitingDialog();

	void SetProgress(const int& progress);

private:
	Ui::WaitingDialog* mpUi;
	QMovie* mMovie;
};
