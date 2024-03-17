#pragma once

#include <QWidget>
#include "ui_NodeInfo.h"


class NodeInfo : public QWidget
{
	Q_OBJECT

public:
	NodeInfo(QWidget* parent = nullptr);
	~NodeInfo();

private:
	Ui::NodeInfoClass ui;
};
