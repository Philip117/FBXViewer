#include <QApplication>
#include "Settings.h"


namespace Settings
{
	Settings::Settings()
	{
		mLanguage = ELanguage::English;
		mFontSize = 9;
	}

	Settings::~Settings()
	{}

	void Settings::LoadSettings()
	{
		QString lPath = QApplication::applicationDirPath() + "/FBXViewer.ini";
		// QSettings::Format::NativeFormat 表示采取最适合对应平台的编码方式
		// QSettings::Format::IniFormat
		QSettings lSettings(QApplication::applicationDirPath() + "/FBXViewer.ini", QSettings::IniFormat);
		// QSetting 读取值时候的第二个参数是当所取的值不存在时返回的默认值
		mLanguage = lSettings.value("Language", QVariant::fromValue<ELanguage>(ELanguage::English))
			.value<ELanguage>();
		mFontSize = lSettings.value("FontSize", 10).value<unsigned int>();
		mLastFileDir = lSettings.value("LastFileDir", "C:/").toString();
	}

	void Settings::SaveSettings()
	{
		QSettings lSettings(QApplication::applicationDirPath() + "/FBXViewer.ini", QSettings::NativeFormat);
		lSettings.setValue("Language", QVariant::fromValue<ELanguage>(mLanguage));
		lSettings.setValue("FontSize", QVariant::fromValue<unsigned int>(mFontSize));
		lSettings.setValue("LastFileDir", mLastFileDir);
		lSettings.sync();
	}

	Settings& Settings::operator=(const Settings& settings)
	{
		mLanguage = settings.mLanguage;
		mFontSize = settings.mFontSize;
		mLastFileDir = settings.mLastFileDir;

		return *this;
	}

	bool Settings::operator==(const Settings& settings)
	{
		return (mLanguage == settings.mLanguage && mFontSize == settings.mFontSize
			&& mLastFileDir == settings.mLastFileDir);
	}

	bool Settings::operator!=(const Settings& settings)
	{
		return (mLanguage != settings.mLanguage || mFontSize != settings.mFontSize
			&& !(mLastFileDir == settings.mLastFileDir));
	}

}