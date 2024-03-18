#include <string>
#include <qfileinfo.h>
#include "../Headers/FileInfo.h"


FileInfo::FileInfo(QWidget* parent)
	: QWidget(parent)
{
	mUi.setupUi(this);
	mpManager = nullptr;
	mpScene = nullptr;

	connect(mUi.listWidget, &QListWidget::pressed, this, &FileInfo::OnPressedListItem);
}

FileInfo::~FileInfo()
{
	mpManager = nullptr;
	mpScene = nullptr;
}


void FileInfo::RefreshUi()
{
	mUi.tableWidget->clear();
	mUi.tableWidget->setRowCount(0);
	switch (mUi.listWidget->currentRow())
	{
	case 0:
		RefreshUi_Basic();
		break;
	case 1:
		RefreshUi_Animation();
		break;
	case 2:
		RefreshUi_Statistic();
		break;
	}
}

void FileInfo::RefreshUi_Basic()
{
	InsertRow("File Name:", mFileName);
	InsertRow("File Path:", mFilePath);
	InsertRow("File Size:", std::to_string(mFileSize) + " KB");
}

void FileInfo::RefreshUi_Animation()
{
	InsertRow("aaafa", "bbbad");
}

void FileInfo::RefreshUi_Statistic()
{
}

void FileInfo::RefreshData()
{
	mFileName = mFilePath.substr(mFilePath.find_last_of("/") + 1, mFilePath.length());
	QFileInfo lQFileInfo(mFilePath.c_str());
	mFileSize = lQFileInfo.size() / 1024;
}


void FileInfo::SetFilePath(const std::string& filePath)
{
	mFilePath = filePath;
}

void FileInfo::SetManagerAndScene(FbxManager* pManager, FbxScene* pScene)
{
	mpManager = pManager;
	mpScene = pScene;
}


void FileInfo::OnPressedListItem(const QModelIndex& index)
{
	RefreshUi();
}

void FileInfo::InsertRow(const std::string& attribute, const std::string& value)
{
	const int lRow = mUi.tableWidget->rowCount();
	QTableWidgetItem* lpItem = nullptr;
	QFont lFont;

	mUi.tableWidget->setRowCount(lRow + 1);
	lpItem = new QTableWidgetItem();
	mUi.tableWidget->setVerticalHeaderItem(lRow, lpItem);
	lpItem = new QTableWidgetItem();
	lpItem->setText(QCoreApplication::translate("FileInfoClass", attribute.c_str(), nullptr));
	lpItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	lFont.setBold(true);
	lpItem->setFont(lFont);
	mUi.tableWidget->setItem(lRow, 0, lpItem);
	lpItem = new QTableWidgetItem();
	lpItem->setText(QCoreApplication::translate("FileInfoClass", value.c_str(), nullptr));
	mUi.tableWidget->setItem(lRow, 1, lpItem);
}

