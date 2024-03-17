#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FBXViewer.h"


class FbxViewer : public QMainWindow
{
	Q_OBJECT

public:
	FbxViewer(QWidget* parent = nullptr);
	~FbxViewer();

private:
	void OpenFbxFile();
	void ExitFbxViewer();

	Ui::FBXViewerClass ui;
};
