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
	void OnAction_OpenFbxFile();
	void OnAction_ExitFbxViewer();
	void OnAction_ViewFileInfo();
	void OnAction_ViewNodeInfo();

	Ui::FBXViewerClass mUi;
	QString mLastFilePath;
};
