#include <QApplication>
#include <QFile>
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
		// �����������õ������ļ����ͼ��ؽ���
		if (QFile::exists(QApplication::applicationDirPath() + "/FBXViewer.ini"))
		{
			// QSettings::Format::NativeFormat ��ʾ��ȡ���ʺ϶�Ӧƽ̨�ı��뷽ʽ
			// QSettings::Format::IniFormat
			QSettings lSettings(QApplication::applicationDirPath() + "/FBXViewer.ini", QSettings::IniFormat);
			lSettings.beginGroup("FBXViewer");
			// QSetting ��ȡֵʱ��ĵڶ��������ǵ���ȡ��ֵ������ʱ���ص�Ĭ��ֵ
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
		lSettings.sync();
		lSettings.endGroup();
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