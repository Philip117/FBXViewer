#include "../Headers/FileInfo.h"


FileInfo::FileInfo(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

FileInfo::~FileInfo()
{}


void FileInfo::SetManager(FbxManager* pManager)
{
	mpManager = pManager;
}

void FileInfo::SetScene(FbxScene* pScene)
{
	mpScene = pScene;
}