#pragma once

#include <QWidget>
#include <qmovie.h>
#include "ui_WaitingWidget.h"

class WaitingWidget : public QWidget
{
	Q_OBJECT

public:
	WaitingWidget(QWidget* parent = nullptr);
	~WaitingWidget();

private:
	Ui::WaitingWidget* mpUi;
	QMovie* mMovie;
};
