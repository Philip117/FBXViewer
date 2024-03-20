#pragma once

#include <QWidget>
#include <fbxsdk.h>
#include <map>
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
	void OnPressedTreeItem(const QModelIndex& index);
	void InsertRow(const std::string& attribute, const std::string& value);

	Ui::NodeInfoClass mUi;
	FbxManager* mpManager;
	FbxScene* mpScene;
	FbxNode* mpRootNode;
	std::map<QTreeWidgetItem*, FbxNode*> mItemToNode;
};
