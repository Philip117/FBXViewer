﻿#include <qfiledialog.h>
#include <qmessagebox.h>
#include <string>
#include <format>
#include <queue>
#include "MainWindow.h"
#include "Others/DataType.h"
#include "Tools/FBX_Base.h"
#include "Tools/FBX_Extraction.h"
#include "Tools/FBX_EnumTransformation.h"


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mpUi(new Ui::MainWindowClass())
{
	mpUi->setupUi(this);
	mpManager = nullptr;
	mpScene = nullptr;
	Settings::Settings lSettings;
	lSettings.LoadSettings();
	mLastFileDir = lSettings.mLastFileDir;
	Fbx_Base::InitializeManagerAndScene(mpManager, mpScene);
	mpWaitingDialog = new WaitingDialog(this, false, true);
	mpSettingsWidget = new SettingsWidget(this);

	connect(mpUi->action_open, &QAction::triggered, this, &MainWindow::Slot_Open);
	connect(mpUi->action_exit, &QAction::triggered, this, &MainWindow::Slot_Exit);
	connect(mpUi->action_statistic, &QAction::triggered, this, &MainWindow::Slot_Statistic);
	connect(mpUi->action_settings, &QAction::triggered, this, &MainWindow::Slot_Settings);
	connect(mpUi->action_about, &QAction::triggered, this, &MainWindow::Slot_About);

	connect(mpSettingsWidget, &SettingsWidget::Signal_LanguageChanged, this, &MainWindow::Slot_Settings_LanguageChanged, Qt::DirectConnection);
	connect(mpSettingsWidget, &SettingsWidget::Signal_FontSizeChanged, this, &MainWindow::Slot_Settings_FontSizeChanged, Qt::DirectConnection);

	connect(mpUi->treeWidget, &QTreeWidget::itemClicked, this,
		static_cast<void(MainWindow::*)(QTreeWidgetItem * pItem, int column)> (&MainWindow::RefreshUi_NodeAttribute));
}

MainWindow::~MainWindow()
{
	delete mpUi;
	Fbx_Base::DestroyScene(mpScene);
	Fbx_Base::DestroyManager(mpManager);
	delete mpWaitingDialog;
}


void MainWindow::RefreshUi()
{
	RefreshUi_TreeWidget();
	RefreshUi_TabWidget();
}

void MainWindow::RefreshUi_TreeWidget()
{
	int lCount = mpUi->treeWidget->topLevelItemCount();
	// 直接 clear() 会奔溃，需要手动释放树节点的内存
	if (mpUi->treeWidget->topLevelItemCount() > 0)
		mpUi->treeWidget->invisibleRootItem()->takeChildren();
	mpUi->treeWidget->clear();

	std::queue<std::pair<FbxNode*, QTreeWidgetItem*>> lQueue;
	FbxNode* lpNode = nullptr;
	QTreeWidgetItem* lpItem = nullptr,
		* lpItem_child = nullptr;
	std::string lNodeName;
	std::vector<FbxNode*> lModelRoots = Fbx_Extraction::GetModelRoots(mpScene);

	lpItem = new QTreeWidgetItem();
	mpUi->treeWidget->addTopLevelItem(lpItem);
	lQueue.push(std::make_pair(mpScene->GetRootNode(), lpItem));
	std::tuple<FbxNode*, QTreeWidgetItem*> lNode;
	while (!lQueue.empty())
	{
		lNode = lQueue.front();
		lQueue.pop();
		lpNode = std::get<0>(lNode);
		lpItem = std::get<1>(lNode);
		lpItem->setText(0, lpNode->GetName());
		lpItem->setData(0, Qt::UserRole, QVariant::fromValue<FbxNode*>(lpNode));
		lNodeName = lpNode->GetName();
		std::transform(lNodeName.begin(), lNodeName.end(), lNodeName.begin(), ::tolower);
		if (lNodeName.find("hip") != std::string::npos)
			lpItem->setForeground(0, QBrush(QColor("blue")));
		for (FbxNode* pNode : lModelRoots)
		{
			if (lpNode == pNode)
				lpItem->setForeground(0, QBrush(QColor("purple")));
		}

		for (int i = 0; i < lpNode->GetChildCount(); i++)
		{
			lpItem_child = new QTreeWidgetItem();
			lpItem->addChild(lpItem_child);
			lQueue.push(std::make_pair(lpNode->GetChild(i), lpItem_child));
		}
	}
}

void MainWindow::RefreshUi_TabWidget()
{
	RefreshUi_NodeAttribute();
}

void MainWindow::RefreshUi_NodeAttribute()
{

}

void MainWindow::RefreshUi_NodeAttribute(QTreeWidgetItem* pItem, int column)
{
	FbxNode* lpNode = pItem->data(column, Qt::UserRole).value<FbxNode*>();
	mpUi->label_nodeName->setText(lpNode->GetName());
	mpUi->lineEdit_nodeType->setText(lpNode->GetTypeName());
	//mpUi->lineEdit_nodeType->setText(Fbx_EnumTransformation::ENodeTypeToString(lpNode->GetTypeName()))
}


void MainWindow::Slot_Open()
{
	std::string lFilePath_std;
	QString lFilePath_qt;
	//if (mLastFileDir.isEmpty())
	//	lFilePath_qt = QFileDialog::getOpenFileName(this, "Open FBX File", "C:/", "*.fbx");
	//else
	//	lFilePath_qt = QFileDialog::getOpenFileName(this, "Open FBX File", mLastFileDir, "*.fbx");
	lFilePath_qt = QFileDialog::getOpenFileName(this, "Open FBX File", mLastFileDir, "*.fbx");

	if (lFilePath_qt != nullptr)
	{
		lFilePath_std = lFilePath_qt.toLocal8Bit().constData();	// 直接 constData() 拿到的是 QChar 类型
		mLastFileDir = lFilePath_qt.mid(0, lFilePath_qt.lastIndexOf("/"));
		Settings::Settings lSettings;
		lSettings.LoadSettings();
		lSettings.mLastFileDir = mLastFileDir;
		lSettings.SaveSettings();

		//qDebug() << lFileName;
		//QMessageBox::information(nullptr, "Title", lFileName, QMessageBox::Yes);

		mpWaitingDialog->show();
		Fbx_Base::TransformFilePath(lFilePath_std);
		if (!Fbx_Base::LoadFbxFile(mpManager, mpScene, lFilePath_std))
			QMessageBox::information(nullptr, "Error", std::format("LoadFbxFile failed\n{}",
				lFilePath_std.c_str()).c_str(), QMessageBox::Yes);
		else
			mLastFileDir = lFilePath_std.c_str();
		mpWaitingDialog->close();
		RefreshUi();
	}
}

void MainWindow::Slot_Exit()
{
	close();
}

void MainWindow::Slot_Statistic()
{

}

void MainWindow::Slot_Settings()
{
	mpSettingsWidget->show();
}

void MainWindow::Slot_About()
{

}


void MainWindow::Slot_Settings_LanguageChanged(const int& index)
{
	Settings::ELanguage lLanguage = static_cast<Settings::ELanguage>(index);	// 选项的下标刚好对应着枚举值
	if (lLanguage == Settings::ELanguage::Chinese)
	{
		if (!mTranslator.load(":/FBXViewer/Translation_zh_CN.qm"))
			return;
	}
	else
	{
		if (!mTranslator.load(":/FBXViewer/Translation_en_US.qm"))
			return;
	}
	qApp->installTranslator(&mTranslator);
	mpUi->retranslateUi(this);
}

void MainWindow::Slot_Settings_FontSizeChanged(const int& fontSize)
{
	QFont lFont = this->font();
	lFont.setPointSize(fontSize);
	this->setFont(lFont);
}
