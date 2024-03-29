#include <fbxsdk.h>
#include <queue>
#include "../Headers/NodeInfo.h"
#include "../Headers/FBX_EnumTransformation.h"
#include "../Headers/Fbx_Extraction.h"


NodeInfo::NodeInfo(QWidget* parent)
	: QWidget(parent)
{
	mUi.setupUi(this);
	mpManager = nullptr;
	mpScene = nullptr;
	mpRootNode = nullptr;

	connect(mUi.treeWidget, &QTreeWidget::pressed, this, &NodeInfo::OnPressedTreeItem);
}

NodeInfo::~NodeInfo()
{
	mpManager = nullptr;
	mpScene = nullptr;
}


void NodeInfo::RefreshUi()
{
	RefreshUi_TreeWidget();
	RefreshUi_TableWidget();
}

void NodeInfo::RefreshUi_TreeWidget()
{
	int lCount = mUi.treeWidget->topLevelItemCount();
	// 直接 clear() 会奔溃，需要手动释放树节点的内存
	if (mUi.treeWidget->topLevelItemCount() > 0)
		mUi.treeWidget->invisibleRootItem()->takeChildren();
	mUi.treeWidget->clear();
	mItemToNode.clear();

	mpRootNode = mpScene->GetRootNode();
	std::queue<std::tuple<FbxNode*, QTreeWidgetItem*>> lQueue;
	FbxNode* lpNode = nullptr;
	QTreeWidgetItem* lpItem = nullptr,
		* lpItem_child = nullptr;
	std::string lNodeName;
	std::vector<FbxNode*> lModelRoots = Fbx_Extraction::GetModelRoots(mpScene);

	lpNode = mpRootNode;
	lpItem = new QTreeWidgetItem();
	mUi.treeWidget->addTopLevelItem(lpItem);
	lQueue.push(std::make_tuple(mpRootNode, lpItem));
	std::tuple<FbxNode*, QTreeWidgetItem*> lNode;
	while (!lQueue.empty())
	{
		lNode = lQueue.front();
		lQueue.pop();
		lpNode = std::get<0>(lNode);
		lpItem = std::get<1>(lNode);
		mItemToNode.insert(std::make_pair(lpItem, lpNode));
		lpItem->setText(0, QCoreApplication::translate("NodeInfoClass", lpNode->GetName(), nullptr));

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
			lQueue.push(std::make_tuple(lpNode->GetChild(i), lpItem_child));
		}
	}
}

void NodeInfo::RefreshUi_TableWidget()
{
	FbxNode* lpNode = mItemToNode[mUi.treeWidget->currentItem()];
	mUi.tableWidget->clear();
	mUi.tableWidget->setRowCount(0);
	if (lpNode)
	{
		FbxNodeAttribute* lpAttribute = lpNode->GetNodeAttribute();
		mUi.tableWidget->clear();
		InsertRow("Node Name:", lpNode->GetName());
		if (lpAttribute)
		{
			FbxNodeAttribute::EType lNodeType;
			lNodeType = lpAttribute->GetAttributeType();
			InsertRow("Node Type:", Fbx_EnumTransformation::ENodeTypeToString(lNodeType));
			//InsertRow("Node Type:", lpAttribute->GetTypeName());

			std::string lNodeName = lpNode->GetName();
			std::transform(lNodeName.begin(), lNodeName.end(), lNodeName.begin(), ::tolower);
			if (lNodeName.find("hip") != std::string::npos && lNodeType != FbxNodeAttribute::EType::eSkeleton)
				mUi.tableWidget->item(1, 1)->setForeground(QBrush(QColor("red")));

			if (lNodeType == FbxNodeAttribute::EType::eSkeleton)
			{
				FbxSkeleton* lpSkeleton = lpNode->GetSkeleton();
				FbxSkeleton::EType lSkeletonType = lpSkeleton->GetSkeletonType();
				InsertRow("Skeleton Type:", Fbx_EnumTransformation::ESkeletonTypeToString(lSkeletonType));
			}
		}
	}
}

void NodeInfo::RefreshData()
{
	mpRootNode = mpScene->GetRootNode();
}

void NodeInfo::SetManagerAndScene(FbxManager* pManager, FbxScene* pScene)
{
	mpManager = pManager;
	mpScene = pScene;
}

void NodeInfo::OnPressedTreeItem(const QModelIndex& index)
{
	RefreshUi_TableWidget();
}

void NodeInfo::InsertRow(const std::string& attribute, const std::string& value)
{
	const int lRow = mUi.tableWidget->rowCount();
	QTableWidgetItem* lpItem = nullptr;
	QFont lFont;

	mUi.tableWidget->setRowCount(lRow + 1);
	lpItem = new QTableWidgetItem();
	mUi.tableWidget->setVerticalHeaderItem(lRow, lpItem);
	lpItem = new QTableWidgetItem();
	lpItem->setText(QCoreApplication::translate("NodeInfoClass", attribute.c_str(), nullptr));
	lpItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	lFont.setBold(true);
	lpItem->setFont(lFont);
	mUi.tableWidget->setItem(lRow, 0, lpItem);
	lpItem = new QTableWidgetItem();
	lpItem->setText(QCoreApplication::translate("NodeInfoClass", value.c_str(), nullptr));
	mUi.tableWidget->setItem(lRow, 1, lpItem);
}

