#pragma once

#include <QWidget>
#include <fbxsdk.h>
#include "ui_FileInfo.h"


class FileInfo : public QWidget
{
	Q_OBJECT

public:
	FileInfo(QWidget* parent = nullptr);
	~FileInfo();

	void SetManager(FbxManager* pManager);
	void SetScene(FbxScene* pScene);

private:
	Ui::FileInfoClass ui;
	FbxManager* mpManager;
	FbxScene* mpScene;
};
