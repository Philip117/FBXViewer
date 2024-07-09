#pragma once

#include <QSettings>


namespace Settings
{
	enum ELanguage
	{
		Chinese = 0,
		English = 1,
		Count
	};

	class Settings
	{
	public:
		Settings();
		~Settings();

		void LoadSettings();
		void SaveSettings();
		Settings& operator=(const Settings& settings);
		bool operator==(const Settings& settings);
		bool operator!=(const Settings& settings);

		ELanguage mLanguage;
		unsigned int mFontSize;
		QString mLastFileDir;
	};
};

