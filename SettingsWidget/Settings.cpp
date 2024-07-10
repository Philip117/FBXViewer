#include <QApplication>
#include <QFile>
#include "Settings.h"


namespace Settings
{
	Settings::Settings()
	{
		mLanguage = ELanguage::Chinese;
		mFontSize = 9;
	}

	Settings::~Settings()
	{}

	void Settings::LoadSettings()
	{
		// 若本地有设置的配置文件，就加载进来
		if (QFile::exists(QApplication::applicationDirPath() + "/FBXViewer.ini"))
		{
			// QSettings::Format::NativeFormat 表示采取最适合对应平台的编码方式
			// QSettings::Format::IniFormat
			QSettings lSettings(QApplication::applicationDirPath() + "/FBXViewer.ini", QSettings::IniFormat);
			lSettings.beginGroup("FBXViewer");
			// QSetting 读取值时候的第二个参数是当所取的值不存在时返回的默认值
			mLanguage = lSettings.value("Language", QVariant::fromValue<ELanguage>(ELanguage::English))
				.value<ELanguage>();
			auto lFontSize = lSettings.value("FontSize");
			mFontSize = lSettings.value("FontSize", 10).value<unsigned int>();
			mLastFileDir = lSettings.value("LastFileDir", "C:/").toString();
			lSettings.endGroup();
		}
	}

	void Settings::SaveSettings()
	{
		QSettings lSettings(QApplication::applicationDirPath() + "/FBXViewer.ini", QSettings::IniFormat);
		lSettings.beginGroup("FBXViewer");
		lSettings.setValue("Language", QVariant::fromValue<ELanguage>(mLanguage));
		lSettings.setValue("FontSize", QVariant::fromValue<unsigned int>(mFontSize));
		lSettings.setValue("LastFileDir", mLastFileDir);
		lSettings.endGroup();
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