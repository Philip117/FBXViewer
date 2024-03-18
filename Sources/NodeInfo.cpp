#include <fbxsdk.h>
#include "../Headers/NodeInfo.h"


NodeInfo::NodeInfo(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mpManager = nullptr;
	mpScene = nullptr;
}

NodeInfo::~NodeInfo()
{
	mpManager = nullptr;
	mpScene = nullptr;
}


void NodeInfo::SetManager(FbxManager* pManager)
{
	mpManager = pManager;
}

void NodeInfo::SetScene(FbxScene* pScene)
{
	mpScene = pScene;
}