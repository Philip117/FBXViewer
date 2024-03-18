#include <qfiledialog.h>
#include <qmessagebox.h>
#include <iostream>
#include <format>
#include "../Headers/FBXViewer.h"
#include "../Headers/Fbx_Common.h"


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

	mpManager = nullptr;
	mpScene = nullptr;
	Fbx_Common::InitializeManagerAndScene(mpManager, mpScene);
	mUi.page_fileInfo->SetManager(mpManager);
	mUi.page_fileInfo->SetScene(mpScene);
	mUi.page_nodeInfo->SetManager(mpManager);
	mUi.page_nodeInfo->SetScene(mpScene);
}

FbxViewer::~FbxViewer()
{
	Fbx_Common::DestroyManager(mpManager);
	mpManager = nullptr;
	mpScene = nullptr;
}

void FbxViewer::OnAction_OpenFbxFile()
{
	mLastFilePath = QFileDialog::getOpenFileName(this, "Open FBX File", "C:/", "*.fbx");
	//qDebug() << lFileName;
	//QMessageBox::information(nullptr, "Title", lFileName, QMessageBox::Yes);
	mUi.page_fileInfo->SetManager(mpManager);
	mUi.page_fileInfo->SetScene(mpScene);

	std::string lFilePath = mLastFilePath.toLocal8Bit().constData();	// 直接 constData() 拿到的是 QChar 类型
	Fbx_Common::TransformFilePath(lFilePath);
	if (!Fbx_Common::LoadFbxFile(mpManager, mpScene, lFilePath))
	{
		QMessageBox::information(nullptr, "Error", std::format("LoadFbxFile failed\n{}", lFilePath.c_str()).c_str(),
			QMessageBox::Yes);
	}
	else
	{
		mUi.page_fileInfo->SetFilePath(lFilePath);
		if (mUi.stackedWidget->currentIndex() == 0)
			mUi.page_fileInfo->RefreshUi();
		//else
		//	mUi.page_nodeInfo->RefreshUi();
	}
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