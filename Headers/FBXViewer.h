#pragma once

#include <QtWidgets/QMainWindow>
#include <fbxsdk.h>
#include "ui_FBXViewer.h"
#include "Waiting.h"


class FbxViewer : public QMainWindow
{
	Q_OBJECT

public:
	FbxViewer(QWidget* parent = nullptr);
	~FbxViewer();

private:
	void OnAction_OpenFbxFile();
	void	LoadFbxFile(const std::string& filePath);
	void OnAction_ExitFbxViewer();
	void OnAction_ViewFileInfo();
	void OnAction_ViewNodeInfo();

	Ui::FBXViewerClass mUi;
	QString mLastFileDir;
	FbxManager* mpManager;
	FbxScene* mpScene;
	Waiting* mpWaiting;
};
