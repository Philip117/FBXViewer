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

	void SetManager(FbxManager* pManager);
	void SetScene(FbxScene* pScene);

private:
	Ui::NodeInfoClass ui;
	FbxManager* mpManager;
	FbxScene* mpScene;
};
