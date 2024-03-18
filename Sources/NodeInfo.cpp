#include <fbxsdk.h>
#include <queue>
#include "../Headers/NodeInfo.h"


NodeInfo::NodeInfo(QWidget* parent)
	: QWidget(parent)
{
	mUi.setupUi(this);
	mpManager = nullptr;
	mpScene = nullptr;
}

NodeInfo::~NodeInfo()
{
	mpManager = nullptr;
	mpScene = nullptr;
}


void NodeInfo::RefreshUi()
{
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
	mpRootNode = mpScene->GetRootNode();
	std::queue<std::tuple<FbxNode*, QTreeWidgetItem*>> lQueue;
	FbxNode* lpNode = nullptr;
	QTreeWidgetItem* lpItem = nullptr,
		* lpItem_child = nullptr;

	lpNode = mpRootNode;
	lpItem = new QTreeWidgetItem();
	mUi.treeWidget->addTopLevelItem(lpItem);
	lQueue.push(std::tuple(mpRootNode, lpItem));
	while (!lQueue.empty())
	{
		std::tuple(lpNode, lpItem) = lQueue.front();
		lQueue.pop();
		lpItem->setText(0, QCoreApplication::translate("NodeInfoClass", lpNode->GetName(), nullptr));
		for (int i = 0; i < lpNode->GetChildCount(); i++)
		{
			lpItem_child = new QTreeWidgetItem();
			lpItem->addChild(lpItem_child);
			lQueue.push(std::tuple(lpNode->GetChild(i), lpItem_child));
		}
	}
}

void NodeInfo::SetManagerAndScene(FbxManager* pManager, FbxScene* pScene)
{
	mpManager = pManager;
	mpScene = pScene;
}
