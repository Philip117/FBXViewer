#pragma once

#include <QWidget>
#include "ui_FileInfo.h"


class FileInfo : public QWidget
{
	Q_OBJECT

public:
	FileInfo(QWidget* parent = nullptr);
	~FileInfo();

private:
	Ui::FileInfoClass ui;
};
