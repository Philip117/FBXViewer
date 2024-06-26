#include <QMessageBox>
#include "SettingsWidget.h"


SettingsWidget::SettingsWidget(QWidget* parent)
	: QWidget(parent)
	, mpUi(new Ui::SettingsWidgetClass())
{
	mpUi->setupUi(this);
	this->setWindowOpacity(0.8);	// 透明度
	setWindowFlags(Qt::Dialog);	// 设置为去掉边框的对话框风格
	mSettings.LoadSettings();
	unsigned int lIndex_language = mSettings.mLanguage == Settings::ELanguage::English ? 0 : 1;
	mpUi->comboBox_language->setCurrentIndex(lIndex_language);
	mpUi->spinBox_fontSize->setValue(mSettings.mFontSize);

	connect(mpUi->pushButton_confirm, &QPushButton::clicked, this, &SettingsWidget::Event_Confirm);
	connect(mpUi->pushButton_reset, &QPushButton::clicked, this, &SettingsWidget::Event_Reset);
	connect(mpUi->pushButton_cancel, &QPushButton::clicked, this, &SettingsWidget::Event_Cancel);
}

SettingsWidget::~SettingsWidget()
{
	delete mpUi;
}


void SettingsWidget::RefreshUi()
{
	unsigned int lIndex_language = mSettings.mLanguage == Settings::ELanguage::English ? 0 : 1;
	mpUi->comboBox_language->setCurrentIndex(lIndex_language);
	mpUi->spinBox_fontSize->setValue(mSettings.mFontSize);
}

void SettingsWidget::closeEvent(QCloseEvent* pEvent)
{
	Settings::Settings lSettings;
	lSettings.LoadSettings();
	if (mSettings != lSettings)
	{
		QMessageBox::StandardButton lStandardButton = QMessageBox::question(this, "Warning", QCoreApplication::translate("SettingsWidgetClass",
			"Settings were changed but not saved, would you like to save them all before close it?",
			nullptr), QMessageBox::Ok | QMessageBox::No);
		if (lStandardButton == QMessageBox::Ok)
			mSettings.SaveSettings();
	}
}

void SettingsWidget::Event_Confirm()
{
	mSettings.mLanguage = static_cast<Settings::ELanguage>(mpUi->comboBox_language->currentIndex());
	mSettings.mFontSize = static_cast<unsigned int>(mpUi->spinBox_fontSize->value());
	mSettings.SaveSettings();
	close();
}

void SettingsWidget::Event_Reset()
{
	Settings::Settings lSettings;
	lSettings.LoadSettings();
	mSettings = lSettings;
	RefreshUi();
}

void SettingsWidget::Event_Cancel()
{
	//Event_Reset();
	close();
}

