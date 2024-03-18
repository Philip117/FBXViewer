#include <fbxsdk.h>
#include "../Headers/NodeInfo.h"


NodeInfo::NodeInfo(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

NodeInfo::~NodeInfo()
{}


void NodeInfo::SetManager(FbxManager* pManager)
{
	mpManager = pManager;
}

void NodeInfo::SetScene(FbxScene* pScene)
{
	mpScene = pScene;
}