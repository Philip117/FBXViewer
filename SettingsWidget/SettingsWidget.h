#pragma once

#include <QWidget>
#include "ui_SettingsWidget.h"
#include "../Tools/Settings.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWidgetClass; };
QT_END_NAMESPACE

class SettingsWidget : public QWidget
{
	Q_OBJECT

public:
	SettingsWidget(QWidget *parent = nullptr);
	~SettingsWidget();

	void RefreshUi();
	void closeEvent(QCloseEvent* pEvent);

	void Event_Confirm();
	void Event_Reset();
	void Event_Cancel();

private:
	Ui::SettingsWidgetClass *mpUi;
	Settings::Settings mSettings;
};
