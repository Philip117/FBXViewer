#include <qfiledialog.h>
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
	mpWidget_rotationAttributes = mpUi->tab_rotationAttributes;
	mpWidget_pivotAttributes = mpUi->tab_pivotAttributes;
	mpUi->tabWidget->removeTab(2);
	mpUi->tabWidget->removeTab(1);
	mpManager = nullptr;
	mpScene = nullptr;
	Settings::Settings lSettings;
	lSettings.LoadSettings();
	mLastFileDir = lSettings.mLastFileDir;
	Fbx_Base::InitializeManagerAndScene(mpManager, mpScene);
	mpWaitingDialog = new WaitingDialog(this, false, true);
	mpSettingsWidget = new SettingsWidget(this);

	RefreshUi_NodeAttributes();

	connect(mpUi->action_open, &QAction::triggered, this, &MainWindow::Slot_Open);
	connect(mpUi->action_exit, &QAction::triggered, this, &MainWindow::Slot_Exit);
	connect(mpUi->action_statistic, &QAction::triggered, this, &MainWindow::Slot_Statistic);
	connect(mpUi->action_settings, &QAction::triggered, this, &MainWindow::Slot_Settings);
	connect(mpUi->action_about, &QAction::triggered, this, &MainWindow::Slot_About);

	connect(mpSettingsWidget, &SettingsWidget::Signal_LanguageChanged, this, &MainWindow::Slot_Settings_LanguageChanged, Qt::DirectConnection);
	connect(mpSettingsWidget, &SettingsWidget::Signal_FontSizeChanged, this, &MainWindow::Slot_Settings_FontSizeChanged, Qt::DirectConnection);

	connect(mpUi->treeWidget, &QTreeWidget::itemClicked, this,
		static_cast<void(MainWindow::*)(QTreeWidgetItem* const pItem, const int& column)> (&MainWindow::RefreshUi_NodeAttributes));

	connect(mpUi->pushButton_translations_coordSwitch, &QPushButton::clicked, this,
		&MainWindow::Slot_BasicAttributes_Translations_CoordSwitch, Qt::DirectConnection);
	connect(mpUi->pushButton_eulers_coordSwitch, &QPushButton::clicked, this,
		&MainWindow::Slot_BasicAttributes_Eulers_CoordSwitch, Qt::DirectConnection);
	connect(mpUi->pushButton_scalings_coordSwitch, &QPushButton::clicked, this,
		&MainWindow::Slot_BasicAttributes_Scalings_CoordSwitch, Qt::DirectConnection);
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
	RefreshUi_NodeAttributes();
}

void MainWindow::RefreshUi_NodeAttributes()
{
	QTreeWidgetItem* lpTreeWidgetItem = mpUi->treeWidget->currentItem();
	if (lpTreeWidgetItem)
	{
		RefreshUi_NodeAttributes(lpTreeWidgetItem, 0);
	}
	else
	{
		mpUi->label_nodeName->setVisible(false);
		SetVisible_Layout(mpUi->horizontalLayout_nodeType, false);
		SetVisible_Layout(mpUi->horizontalLayout_skeletonType, false);
		SetVisible_Layout(mpUi->horizontalLayout_translations, false);
		SetVisible_Layout(mpUi->horizontalLayout_eulers, false);
		SetVisible_Layout(mpUi->horizontalLayout_scalings, false);
	}
}

void MainWindow::RefreshUi_NodeAttributes(QTreeWidgetItem* const pItem, const int& column)
{
	RefreshUi_NodeAttributes_BasicAttributes(pItem, column);
	FbxNode* lpNode = pItem->data(column, Qt::UserRole).value<FbxNode*>();
	FbxNodeAttribute* lpNodeAttribute = lpNode->GetNodeAttribute();
	if (lpNodeAttribute && (lpNodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton
		|| lpNodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh))
	{
		if(!mpUi->tabWidget->isTabVisible(1))
		mpUi->tabWidget->addTab(mpWidget_rotationAttributes,"Rotation Attributes");
		if (!mpUi->tabWidget->isTabVisible(2))
			mpUi->tabWidget->addTab(mpWidget_pivotAttributes, "Pivot Related");
		RefreshUi_NodeAttributes_RotationAttributes(pItem, column);
		RefreshUi_NodeAttributes_PivotAttributes(pItem, column);
	}
	else
	{
		mpUi->tabWidget->removeTab(2);
		mpUi->tabWidget->removeTab(1);
	}
}

void MainWindow::RefreshUi_NodeAttributes_BasicAttributes(QTreeWidgetItem* const pItem, const int& column)
{
	FbxNode* lpNode = pItem->data(column, Qt::UserRole).value<FbxNode*>();
	mpUi->label_nodeName->setVisible(true);
	mpUi->label_nodeName->setText(lpNode->GetName());

	FbxNodeAttribute* lpNodeAttribute = lpNode->GetNodeAttribute();
	if (lpNodeAttribute)
	{
		SetVisible_Layout(mpUi->horizontalLayout_nodeType, true);
		mpUi->lineEdit_nodeType->setText(QString::fromStdString(
			Fbx_EnumTransformation::ENodeTypeToString(lpNodeAttribute->GetAttributeType())));
		if (lpNodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton)
		{
			SetVisible_Layout(mpUi->horizontalLayout_skeletonType, true);
			mpUi->lineEdit_skeletonType->setText(QString::fromStdString(
				Fbx_EnumTransformation::ESkeletonTypeToString(lpNode->GetSkeleton()->GetSkeletonType())));
		}
		else
		{
			SetVisible_Layout(mpUi->horizontalLayout_skeletonType, false);
		}

		SetVisible_Layout(mpUi->horizontalLayout_translations, true);
		SetVisible_Layout(mpUi->horizontalLayout_eulers, true);
		SetVisible_Layout(mpUi->horizontalLayout_scalings, true);

		FbxDouble3 lTranslations,
			lEulers,
			lScalings;
		FbxAMatrix lTransform = lpNode->EvaluateLocalTransform();
		if (mpUi->pushButton_translations_coordSwitch->text() == "L")
			lTranslations = lTransform.GetT();
		if (mpUi->pushButton_eulers_coordSwitch->text() == "L")
			lTranslations = lTransform.GetT();
		if(mpUi->pushButton_scalings_coordSwitch->text() == "L")
			lScalings = lTransform.GetS();
		lTransform = lpNode->EvaluateGlobalTransform();
		if (mpUi->pushButton_translations_coordSwitch->text() == "W")
			lTranslations = lTransform.GetT();
		if (mpUi->pushButton_eulers_coordSwitch->text() == "W")
			lTranslations = lTransform.GetT();
		if (mpUi->pushButton_scalings_coordSwitch->text() == "W")
			lScalings = lTransform.GetS();

		mpUi->lineEdit_translationX->setText(QString::number(lTranslations[0]));
		mpUi->lineEdit_translationY->setText(QString::number(lTranslations[1]));
		mpUi->lineEdit_translationZ->setText(QString::number(lTranslations[2]));
		mpUi->lineEdit_eulerX->setText(QString::number(lEulers[0]));
		mpUi->lineEdit_eulerY->setText(QString::number(lEulers[1]));
		mpUi->lineEdit_eulerZ->setText(QString::number(lEulers[2]));
		mpUi->lineEdit_scalingX->setText(QString::number(lScalings[0]));
		mpUi->lineEdit_scalingY->setText(QString::number(lScalings[1]));
		mpUi->lineEdit_scalingZ->setText(QString::number(lScalings[2]));

		SetVisible_Layout(mpUi->horizontalLayout_inheritType, true);

		FbxTransform::EInheritType lInheritType;
		lpNode->GetTransformationInheritType(lInheritType);
		mpUi->lineEdit_inheritType->setText(QString::fromStdString(
			Fbx_EnumTransformation::EInheritTypeToString(lInheritType)));
	}
	else
	{
		SetVisible_Layout(mpUi->horizontalLayout_nodeType, false);
		SetVisible_Layout(mpUi->horizontalLayout_skeletonType, false);
		SetVisible_Layout(mpUi->horizontalLayout_translations, false);
		SetVisible_Layout(mpUi->horizontalLayout_eulers, false);
		SetVisible_Layout(mpUi->horizontalLayout_scalings, false);
		SetVisible_Layout(mpUi->horizontalLayout_inheritType, false);

	}
}

void MainWindow::RefreshUi_NodeAttributes_RotationAttributes(QTreeWidgetItem* const pItem, const int& column)
{
	FbxNode* lpNode = pItem->data(column, Qt::UserRole).value<FbxNode*>();
	mpUi->label_nodeName->setText(lpNode->GetName());
	FbxNodeAttribute* lpNodeAttribute = lpNode->GetNodeAttribute();
	if (lpNodeAttribute)
	{
		FbxNode::EPivotSet lPivotSet = FbxNode::eSourcePivot;
		
		FbxEuler::EOrder lOrder;
		lOrder = lpNode->RotationOrder.Get();
		mpUi->lineEdit_rotationOrder->setText(QString::fromStdString(
			Fbx_EnumTransformation::EOrderToString(lOrder)));

		FbxDouble3 lDouble3 = lpNode->LclRotation.Get();
		mpUi->lineEdit_lclRotationX->setText(QString::number(lDouble3[0]));
		mpUi->lineEdit_lclRotationY->setText(QString::number(lDouble3[1]));
		mpUi->lineEdit_lclRotationZ->setText(QString::number(lDouble3[2]));
		lDouble3 = lpNode->RotationOffset.Get();
		mpUi->lineEdit_rotationOffsetX->setText(QString::number(lDouble3[0]));
		mpUi->lineEdit_rotationOffsetY->setText(QString::number(lDouble3[1]));
		mpUi->lineEdit_rotationOffsetZ->setText(QString::number(lDouble3[2]));
		lDouble3 = lpNode->GeometricRotation.Get();
		mpUi->lineEdit_geometricRotationX->setText(QString::number(lDouble3[0]));
		mpUi->lineEdit_geometricRotationY->setText(QString::number(lDouble3[1]));
		mpUi->lineEdit_geometricRotationZ->setText(QString::number(lDouble3[2]));
		lDouble3 = lpNode->PreRotation.Get();
		mpUi->lineEdit_preRotationX->setText(QString::number(lDouble3[0]));
		mpUi->lineEdit_preRotationY->setText(QString::number(lDouble3[1]));
		mpUi->lineEdit_preRotationZ->setText(QString::number(lDouble3[2]));
		lDouble3 = lpNode->PostRotation.Get();
		mpUi->lineEdit_postRotationX->setText(QString::number(lDouble3[0]));
		mpUi->lineEdit_postRotationY->setText(QString::number(lDouble3[1]));
		mpUi->lineEdit_postRotationZ->setText(QString::number(lDouble3[2]));

		FbxLimits lLimits= lpNode->GetRotationLimits();
		bool lIsActive = lLimits.GetActive(),
			lIsActives[3] = { false };
		lLimits.GetMaxActive(lIsActives[0], lIsActives[1], lIsActives[2]);
		lLimits.GetMinActive(lIsActives[0], lIsActives[1], lIsActives[2]);
		lDouble3 = lLimits.GetMax();
		lDouble3 = lLimits.GetMin();
		int a = 0;
	}
}

void MainWindow::RefreshUi_NodeAttributes_PivotAttributes(QTreeWidgetItem* const pItem, const int& column)
{
	FbxNode* lpNode = pItem->data(column, Qt::UserRole).value<FbxNode*>();
	FbxNodeAttribute* lpNodeAttribute = lpNode->GetNodeAttribute();
	if (lpNodeAttribute)
	{
		FbxNode::EPivotSet lPivotSet = FbxNode::eSourcePivot;
		FbxVector4 lVector4;
		FbxDouble3 lDouble3;
		// FbxNode::GetPivots() 得到的 Pivots，由 Pivots::GetRotationOffset(id) 和
		// FbxNode::GetRotationOffset(pivotSet) 好像不是一个东西
		// FbxNode::Pivots lPivots = lpNode->GetPivots();
		// lPivots.GetRotationOffset(0);

		lPivotSet = FbxNode::EPivotSet::eSourcePivot;
		FbxNode::EPivotState lPivotState;
		lpNode->GetPivotState(lPivotSet, lPivotState);
		mpUi->lineEdit_sourcePivot_pivotState->setText(QString::fromStdString(
			Fbx_EnumTransformation::EPivotStateToString(lPivotState)));
		lVector4 = lpNode->GetRotationOffset(lPivotSet);
		mpUi->lineEdit_sourcePivot_rotationOffsetX->setText(QString::number(lVector4[0]));
		mpUi->lineEdit_sourcePivot_rotationOffsetY->setText(QString::number(lVector4[1]));
		mpUi->lineEdit_sourcePivot_rotationOffsetZ->setText(QString::number(lVector4[2]));
		lVector4 = lpNode->GetGeometricRotation(lPivotSet);
		mpUi->lineEdit_sourcePivot_geometricRotationX->setText(QString::number(lVector4[0]));
		mpUi->lineEdit_sourcePivot_geometricRotationY->setText(QString::number(lVector4[1]));
		mpUi->lineEdit_sourcePivot_geometricRotationZ->setText(QString::number(lVector4[2]));
		lVector4 = lpNode->GetPreRotation(lPivotSet);
		mpUi->lineEdit_sourcePivot_preRotationX->setText(QString::number(lVector4[0]));
		mpUi->lineEdit_sourcePivot_preRotationY->setText(QString::number(lVector4[1]));
		mpUi->lineEdit_sourcePivot_preRotationZ->setText(QString::number(lVector4[2]));
		lVector4 = lpNode->GetPostRotation(lPivotSet);
		mpUi->lineEdit_sourcePivot_postRotationX->setText(QString::number(lVector4[0]));
		mpUi->lineEdit_sourcePivot_postRotationY->setText(QString::number(lVector4[1]));
		mpUi->lineEdit_sourcePivot_postRotationZ->setText(QString::number(lVector4[2]));

		lPivotSet = FbxNode::EPivotSet::eDestinationPivot;
		lpNode->GetPivotState(lPivotSet, lPivotState);
		mpUi->lineEdit_destinationPivot_pivotState->setText(QString::fromStdString(
			Fbx_EnumTransformation::EPivotStateToString(lPivotState)));
		lVector4 = lpNode->GetRotationOffset(lPivotSet);
		mpUi->lineEdit_destinationPivot_rotationOffsetX->setText(QString::number(lVector4[0]));
		mpUi->lineEdit_destinationPivot_rotationOffsetY->setText(QString::number(lVector4[1]));
		mpUi->lineEdit_destinationPivot_rotationOffsetZ->setText(QString::number(lVector4[2]));
		lVector4 = lpNode->GetGeometricRotation(lPivotSet);
		mpUi->lineEdit_destinationPivot_geometricRotationX->setText(QString::number(lVector4[0]));
		mpUi->lineEdit_destinationPivot_geometricRotationY->setText(QString::number(lVector4[1]));
		mpUi->lineEdit_destinationPivot_geometricRotationZ->setText(QString::number(lVector4[2]));
		lVector4 = lpNode->GetPreRotation(lPivotSet);
		mpUi->lineEdit_destinationPivot_preRotationX->setText(QString::number(lVector4[0]));
		mpUi->lineEdit_destinationPivot_preRotationY->setText(QString::number(lVector4[1]));
		mpUi->lineEdit_destinationPivot_preRotationZ->setText(QString::number(lVector4[2]));
		lVector4 = lpNode->GetPostRotation(lPivotSet);
		mpUi->lineEdit_destinationPivot_postRotationX->setText(QString::number(lVector4[0]));
		mpUi->lineEdit_destinationPivot_postRotationY->setText(QString::number(lVector4[1]));
		mpUi->lineEdit_destinationPivot_postRotationZ->setText(QString::number(lVector4[2]));
	}
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

void MainWindow::Slot_BasicAttributes_Translations_CoordSwitch()
{
	if (mpUi->pushButton_translations_coordSwitch->text() == "L")
		mpUi->pushButton_translations_coordSwitch->setText("W");
	else
		mpUi->pushButton_translations_coordSwitch->setText("L");
	RefreshUi_NodeAttributes_BasicAttributes(mpUi->treeWidget->currentItem(), 0);
}

void MainWindow::Slot_BasicAttributes_Eulers_CoordSwitch()
{
	if (mpUi->pushButton_eulers_coordSwitch->text() == "L")
		mpUi->pushButton_eulers_coordSwitch->setText("W");
	else
		mpUi->pushButton_eulers_coordSwitch->setText("L");
	RefreshUi_NodeAttributes_BasicAttributes(mpUi->treeWidget->currentItem(), 0);
}

void MainWindow::Slot_BasicAttributes_Scalings_CoordSwitch()
{
	if (mpUi->pushButton_scalings_coordSwitch->text() == "L")
		mpUi->pushButton_scalings_coordSwitch->setText("W");
	else
		mpUi->pushButton_scalings_coordSwitch->setText("L");
	RefreshUi_NodeAttributes_BasicAttributes(mpUi->treeWidget->currentItem(), 0);
}


void MainWindow::SetVisible_Layout(QLayout* const pLayout, const bool& visible)
{
	if (!pLayout)
		return;
	for (int i = 0, count = pLayout->count(); i < count; i++)
	{
		if (pLayout->itemAt(i))
		{
			if (pLayout->itemAt(i)->widget())
				pLayout->itemAt(i)->widget()->setVisible(visible);
			if (pLayout->itemAt(i)->layout())
			{
				SetVisible_Layout(pLayout->itemAt(i)->layout(), visible);
			}
		}
	}
}

