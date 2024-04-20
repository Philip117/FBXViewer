#pragma once

#include <QMainWindow>
#include <fbxsdk.h>
#include "ui_MainWindow.h"
#include "../WaitingWidget/WaitingWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	void RefreshUi();
	void RefreshUi_TreeWidget();
	void RefreshUi_TabWidget();
	void	RefreshUi_NodeAttribute();
	void	RefreshUi_NodeAttribute(QTreeWidgetItem* pItem, int column);

	void OnAction_Open();
	void OnAction_Exit();
	void OnAction_Statistic();
	void OnAction_About();

	Ui::MainWindow* mpUi;
	FbxManager* mpManager;
	FbxScene* mpScene;
	QString mLastFileDir;
	WaitingWidget* mpWaitingWidget;
};
