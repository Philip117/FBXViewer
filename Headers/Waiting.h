#pragma once

#include <QWidget>
#include <qmovie.h>
#include "ui_Waiting.h"

class Waiting : public QWidget
{
	Q_OBJECT

public:
	Waiting(QWidget* parent = nullptr);
	~Waiting();

private:
	Ui::WaitingClass mUi;
	QMovie* mMovie;
};
