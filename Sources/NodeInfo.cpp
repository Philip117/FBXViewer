#include <fbxsdk.h>
#include <queue>
#include "../Headers/NodeInfo.h"


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
		for (int i = 0; i < lpNode->GetChildCount(); i++)
		{
			lpItem_child = new QTreeWidgetItem();
			lpItem->addChild(lpItem_child);
			lQueue.push(std::make_tuple(lpNode->GetChild(i), lpItem_child));
		}
	}
}

void NodeInfo::RefreshUi_Basic()
{
}

void NodeInfo::RefreshUi_Animation()
{
}

void NodeInfo::RefreshUi_Statistic()
{
}

void NodeInfo::RefreshData()
{
}

void NodeInfo::SetManagerAndScene(FbxManager* pManager, FbxScene* pScene)
{
	mpManager = pManager;
	mpScene = pScene;
}

void NodeInfo::OnPressedTreeItem(const QModelIndex& index)
{
	FbxNode* lpNode = mItemToNode[mUi.treeWidget->currentItem()];
	InsertRow("Node Name:", lpNode->GetName());
	if (lpNode->GetNodeAttribute())
		InsertRow("Node Name:", lpNode->GetNodeAttribute()->GetA);
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

