#include <qfiledialog.h>
#include <qmessagebox.h>
#include <iostream>
#include "../Headers/FBXViewer.h"


FbxViewer::FbxViewer(QWidget* parent)
	: QMainWindow(parent)
{
	mUi.setupUi(this);
	// 新式或老式写法都不管 public/protected/private，但老式写法要求声明为 public/protected/private slots
	connect(mUi.action_open, &QAction::triggered, this, &FbxViewer::OnAction_OpenFbxFile);	// 新式写法
	//connect(mUi.action_open, SIGNAL(triggered()), this, SLOT(OpenFbxFile()));	// 老式写法
	connect(mUi.action_exit, &QAction::triggered, this, &FbxViewer::OnAction_ExitFbxViewer);
	connect(mUi.action_fileInfo, &QAction::triggered, this, &FbxViewer::OnAction_ViewFileInfo);
	connect(mUi.action_nodeInfo, &QAction::triggered, this, &FbxViewer::OnAction_ViewNodeInfo);
}

FbxViewer::~FbxViewer()
{}

void FbxViewer::OnAction_OpenFbxFile()
{
	mLastFilePath = QFileDialog::getOpenFileName(this, "Open FBX File", "C:/", "*.fbx");
	//qDebug() << lFileName;
	//QMessageBox::information(nullptr, "Title", lFileName, QMessageBox::Yes);
}

void FbxViewer::OnAction_ExitFbxViewer()
{
	close();
}

void FbxViewer::OnAction_ViewFileInfo()
{
	mUi.stackedWidget->setCurrentWidget(mUi.page_fileInfo);
}

void FbxViewer::OnAction_ViewNodeInfo()
{
	mUi.stackedWidget->setCurrentWidget(mUi.page_nodeInfo);
}