#pragma once

#include <QWidget>
#include <fbxsdk.h>
#include "ui_NodeInfo.h"


class NodeInfo : public QWidget
{
	Q_OBJECT

public:
	NodeInfo(QWidget* parent = nullptr);
	~NodeInfo();

	void RefreshUi();
	void	RefreshUi_Basic();
	void	RefreshUi_Animation();
	void	RefreshUi_Statistic();
	void RefreshData();

	void SetManagerAndScene(FbxManager* pManager, FbxScene* pScene);

private:
	Ui::NodeInfoClass mUi;
	FbxManager* mpManager;
	FbxScene* mpScene;
	FbxNode* mpRootNode;
};
