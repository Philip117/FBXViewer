#include <qmessagebox.h>
#include <qtranslator.h>
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
	Slot_LanguageChanged(lIndex_language);
	Slot_FontSizeChanged(mSettings.mFontSize);

	connect(mpUi->pushButton_confirm, &QPushButton::clicked, this, &SettingsWidget::Slot_Confirm);
	connect(mpUi->pushButton_reset, &QPushButton::clicked, this, &SettingsWidget::Slot_Reset);
	connect(mpUi->pushButton_cancel, &QPushButton::clicked, this, &SettingsWidget::Slot_Cancel);
}

SettingsWidget::~SettingsWidget()
{
	delete mpUi;
}


void SettingsWidget::RefreshUi()
{
	int lIndex_language = mSettings.mLanguage == Settings::ELanguage::Chinese ? 0 : 1;
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

void SettingsWidget::Slot_Confirm()
{
	int lIndex_language = mpUi->comboBox_language->currentIndex();
	mSettings.mLanguage = static_cast<Settings::ELanguage>(lIndex_language);
	mSettings.mFontSize = static_cast<unsigned int>(mpUi->spinBox_fontSize->value());
	Slot_LanguageChanged(lIndex_language);
	Slot_FontSizeChanged(mpUi->spinBox_fontSize->value());
	Signal_LanguageChanged(lIndex_language);
	Signal_FontSizeChanged(mpUi->spinBox_fontSize->value());
	mSettings.SaveSettings();
	close();
}

void SettingsWidget::Slot_Reset()
{
	Settings::Settings lSettings;
	lSettings.LoadSettings();
	mSettings = lSettings;
	RefreshUi();
}

void SettingsWidget::Slot_Cancel()
{
	close();
}


void SettingsWidget::Slot_LanguageChanged(const int& index)
{
	Settings::ELanguage lLanguage = static_cast<Settings::ELanguage>(index);
	if (lLanguage == Settings::ELanguage::Chinese)
	{
		if (!mTranslator.load(":/FBXViewer/Translation_zh_CN.qm"))
			return;
	}
	else
	{
		if (!mTranslator.load(":/FBXViewer/Translation_en_US.qm"))
			return;
	}
	qApp->installTranslator(&mTranslator);
	mpUi->retranslateUi(this);
}

void SettingsWidget::Slot_FontSizeChanged(const int& fontSize)
{
	QFont lFont = this->font();
	lFont.setPointSize(fontSize);
	this->setFont(lFont);
}

