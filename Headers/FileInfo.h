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

	void RefreshUi();
	void	RefreshUi_Basic();
	void	RefreshUi_Animation();
	void	RefreshUi_Statistic();

	void SetFilePath(const std::string& filePath);
	void SetManager(FbxManager* pManager);
	void SetScene(FbxScene* pScene);

private:
	void OnPressedListItem(const QModelIndex& index);
	void InsertRow(const std::string& attribute, const std::string& value);

	Ui::FileInfoClass mUi;
	std::string mFilePath;
	std::string mFileName;
	qint64 mFileSize;
	FbxManager* mpManager;
	FbxScene* mpScene;
};
