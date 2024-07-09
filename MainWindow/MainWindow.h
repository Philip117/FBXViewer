#pragma once

#include <qmainwindow.h>
#include <qtranslator.h>
#include <fbxsdk.h>
#include "ui_MainWindow.h"
#include "WaitingDialog/WaitingDialog.h"
#include "SettingsWidget/SettingsWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	void RefreshUi();
	void	RefreshUi_TreeWidget();
	void	RefreshUi_TabWidget();
	void	RefreshUi_NodeAttribute();
	void	RefreshUi_NodeAttribute(QTreeWidgetItem* pItem, int column);

public slots:
	void Slot_Open();
	void Slot_Exit();
	void Slot_Statistic();
	void Slot_Settings();
	void Slot_About();

	void Slot_Settings_LanguageChanged(const int& index);
	void Slot_Settings_FontSizeChanged(const int& fontSize);

private:

	Ui::MainWindowClass* mpUi;
	FbxManager* mpManager;
	FbxScene* mpScene;
	QString mLastFileDir;
	WaitingDialog* mpWaitingDialog;

	SettingsWidget* mpSettingsWidget;
	QTranslator mTranslator;

};
