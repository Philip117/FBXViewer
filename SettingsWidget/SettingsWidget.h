#pragma once

#include <qwidget.h>
#include <qtranslator.h>
#include "ui_SettingsWidget.h"
#include "Settings.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWidgetClass; };
QT_END_NAMESPACE

class SettingsWidget : public QWidget
{
	Q_OBJECT

public:
	SettingsWidget(QWidget* parent = nullptr);
	~SettingsWidget();

	void RefreshUi();
	void closeEvent(QCloseEvent* pEvent);

public slots:
	void Slot_Confirm();
	void Slot_Reset();
	void Slot_Cancel();

signals:
	void Signal_LanguageChanged(const int& index);
	void Signal_FontSizeChanged(const int& fontSize);
public slots:
	void Slot_LanguageChanged(const int& index);
	void Slot_FontSizeChanged(const int& fontSize);

private:
	Ui::SettingsWidgetClass* mpUi;
	Settings::Settings mSettings;
	QTranslator mTranslator;
};
