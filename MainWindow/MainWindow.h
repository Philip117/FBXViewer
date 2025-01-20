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
	void	RefreshUi_NodeAttributes();
	void	RefreshUi_NodeAttributes(QTreeWidgetItem* const pItem, const int& column);
	void		RefreshUi_NodeAttributes_BasicAttributes(QTreeWidgetItem* const pItem, const int& column);
	void		RefreshUi_NodeAttributes_RotationAttributes(QTreeWidgetItem* const pItem, const int& column);
	void		RefreshUi_NodeAttributes_PivotAttributes(QTreeWidgetItem* const pItem, const int& column);

public slots:
	void Slot_Open();
	void Slot_Exit();
	void Slot_Statistic();
	void Slot_Settings();
	void Slot_About();

	void Slot_Settings_LanguageChanged(const int& index);
	void Slot_Settings_FontSizeChanged(const int& fontSize);

	void Slot_BasicAttributes_Translations_CoordSwitch();
	void Slot_BasicAttributes_Eulers_CoordSwitch();
	void Slot_BasicAttributes_Scalings_CoordSwitch();

private:
	void SetVisible_Layout(QLayout* const pLayout, const bool& visible);

	Ui::MainWindowClass* mpUi;
	QWidget* mpWidget_rotationAttributes;
	QWidget* mpWidget_pivotAttributes;
	FbxManager* mpManager;
	FbxScene* mpScene;
	QString mLastFileDir;
	WaitingDialog* mpWaitingDialog;

	SettingsWidget* mpSettingsWidget;
	QTranslator mTranslator;

};
