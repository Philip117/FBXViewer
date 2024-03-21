#include <qfiledialog.h>
#include <qmessagebox.h>
#include <iostream>
#include <format>
#include <thread>
#include "../Headers/FBXViewer.h"
#include "../Headers/FBX_Common.h"
#include "../Headers/Waiting.h"

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
	mUi.page_fileInfo->SetManagerAndScene(mpManager, mpScene);
	mUi.page_nodeInfo->SetManagerAndScene(mpManager, mpScene);
	mpWaiting = new Waiting(this);
}

FbxViewer::~FbxViewer()
{
	Fbx_Common::DestroyManager(mpManager);
	mpManager = nullptr;
	mpScene = nullptr;
	delete mpWaiting;
}

void FbxViewer::OnAction_OpenFbxFile()
{
	std::string lFilePath_std;
	QString lFilePath_qt;
	if (mLastFileDir.isEmpty())
		lFilePath_qt = QFileDialog::getOpenFileName(this, "Open FBX File", "C:/", "*.fbx");
	else
		lFilePath_qt = QFileDialog::getOpenFileName(this, "Open FBX File", mLastFileDir, "*.fbx");

	if (lFilePath_qt != nullptr)
	{
		lFilePath_std = lFilePath_qt.toLocal8Bit().constData();	// 直接 constData() 拿到的是 QChar 类型
		mLastFileDir = lFilePath_qt.mid(0, lFilePath_qt.lastIndexOf("/"));

		//qDebug() << lFileName;
		//QMessageBox::information(nullptr, "Title", lFileName, QMessageBox::Yes);

		mpWaiting->show();
		Fbx_Common::TransformFilePath(lFilePath_std);

		// 开个线程读 FBX 文件，避免文件太大，读取很久
		std::thread lThread(&FbxViewer::LoadFbxFile, this, lFilePath_std);
		lThread.detach();
	}
}

void FbxViewer::LoadFbxFile(const std::string& filePath)
{
	if (!Fbx_Common::LoadFbxFile(mpManager, mpScene, filePath))
	{
		QMessageBox::information(nullptr, "Error", std::format("LoadFbxFile failed\n{}", filePath.c_str()).c_str(),
			QMessageBox::Yes);
	}
	else
	{
		mUi.page_fileInfo->SetFilePath(filePath);
		if (mUi.stackedWidget->currentIndex() == 0)
		{
			mUi.page_fileInfo->RefreshData();
			mUi.page_fileInfo->RefreshUi();
			//std::thread lThread([](NodeInfo* pNodeInfo) {pNodeInfo->RefreshData(); }, mUi.page_nodeInfo);
			//lThread.join();
			mUi.page_nodeInfo->RefreshData();
		}
		else
		{
			mUi.page_nodeInfo->RefreshData();
			mUi.page_nodeInfo->RefreshUi();
			//std::thread lThread([](FileInfo* pFileInfo) {pFileInfo->RefreshData(); }, mUi.page_fileInfo);
			//lThread.join();
			mUi.page_fileInfo->RefreshData();
		}
	}
	mpWaiting->close();
}

void FbxViewer::OnAction_ExitFbxViewer()
{
	close();
}

void FbxViewer::OnAction_ViewFileInfo()
{
	mUi.stackedWidget->setCurrentWidget(mUi.page_fileInfo);
	mUi.page_fileInfo->RefreshUi();
}

void FbxViewer::OnAction_ViewNodeInfo()
{
	mUi.stackedWidget->setCurrentWidget(mUi.page_nodeInfo);
	mUi.page_nodeInfo->RefreshUi();
}
